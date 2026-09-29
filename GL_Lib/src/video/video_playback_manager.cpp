#include "video_playback_manager.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <string>
#include <utility>
#include <iostream>
#include <cstring>
#include <libavutil/imgutils.h>
#include <libavutil/mem.h>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/error.h>
#include <libswscale/swscale.h>
}

using namespace gllib;

namespace {

std::string ffmpegErrorString(int errorCode) {
    char buffer[AV_ERROR_MAX_STRING_SIZE] = {};

    if (av_strerror(errorCode, buffer, sizeof(buffer)) < 0) {
        return "unknown FFmpeg error";
    }

    return buffer;
}

}

struct VideoPlaybackManager::DecoderState {
    AVFormatContext* formatContext = nullptr;
    AVCodecContext* codecContext = nullptr;
    AVPacket* packet = nullptr;
    AVFrame* decodedFrame = nullptr;
    SwsContext* scaleContext = nullptr;

    int videoStreamIndex = -1;
    AVRational timeBase{0, 1};

    double playbackTime = 0.0;
    double frameDurationFallback = 0.0;
    double fallbackNextTime = 0.0;
    double timestampOrigin = 0.0;

    bool hasTimestampOrigin = false;
    bool inputEnded = false;
    bool flushSent = false;
    bool decoderEnded = false;
    bool opened = false;
    bool playing = false;
    bool finished = false;

    bool hasNextFrame = false;
    DecodedVideoFrame nextFrame;

    ~DecoderState() {
        close();
    }

    void close() {
        if (scaleContext) {
            sws_freeContext(scaleContext);
            scaleContext = nullptr;
        }

        av_frame_free(&decodedFrame);
        av_packet_free(&packet);
        avcodec_free_context(&codecContext);

        if (formatContext) {
            avformat_close_input(&formatContext);
        }

        videoStreamIndex = -1;
        timeBase = AVRational{0, 1};

        playbackTime = 0.0;
        frameDurationFallback = 0.0;
        fallbackNextTime = 0.0;
        timestampOrigin = 0.0;

        hasTimestampOrigin = false;
        inputEnded = false;
        flushSent = false;
        decoderEnded = false;
        opened = false;
        playing = false;
        finished = false;

        hasNextFrame = false;
        nextFrame = DecodedVideoFrame{};
    }

    bool decodeNextFrame(DecodedVideoFrame& output) {
        while (!decoderEnded) {
            int result = avcodec_receive_frame(codecContext, decodedFrame);

            if (result == 0) {
                convertDecodedFrame(output);
                av_frame_unref(decodedFrame);
                return true;
            }

            if (result == AVERROR_EOF) {
                decoderEnded = true;
                return false;
            }

            if (result != AVERROR(EAGAIN)) {
                throw std::runtime_error("Video decoding failed: " + ffmpegErrorString(result));
            }

            if (inputEnded) {
                if (!flushSent) {
                    result = avcodec_send_packet(codecContext, nullptr);
                    if (result < 0 && result != AVERROR_EOF) {
                        throw std::runtime_error("Could not flush video decoder: " + ffmpegErrorString(result));
                    }

                    flushSent = true;
                    continue;
                }

                throw std::runtime_error("Video decoder requested input after its end-of-file flush.");
            }

            while (true) {
                result = av_read_frame(formatContext, packet);

                if (result == AVERROR_EOF) {
                    inputEnded = true;
                    break;
                }

                if (result < 0) {
                    throw std::runtime_error("Could not read video packet: " + ffmpegErrorString(result));
                }

                if (packet->stream_index != videoStreamIndex) {
                    av_packet_unref(packet);
                    continue;
                }

                result = avcodec_send_packet(codecContext, packet);
                av_packet_unref(packet);

                if (result < 0) {
                    throw std::runtime_error("Could not send packet to video decoder: " + ffmpegErrorString(result));
                }

                break;
            }
        }

        return false;
    }

    void convertDecodedFrame(DecodedVideoFrame& output) {
        const int width = decodedFrame->width;
        const int height = decodedFrame->height;

        if (width <= 0 || height <= 0 || width > std::numeric_limits<int>::max() / 4) {
            throw std::runtime_error("Decoder produced invalid video dimensions.");
        }

        const std::size_t rowBytes = static_cast<std::size_t>(width) * 4;
        const std::size_t maxSize = (std::numeric_limits<std::size_t>::max)();

        if (rowBytes > maxSize / static_cast<std::size_t>(height)) {
            throw std::runtime_error("Decoded video frame is too large.");
        }

        output.width = width;
        output.height = height;
        output.rgbaPixels.resize(rowBytes * static_cast<std::size_t>(height));

        scaleContext = sws_getCachedContext(
            scaleContext,
            width,
            height,
            static_cast<AVPixelFormat>(decodedFrame->format),
            width,
            height,
            AV_PIX_FMT_RGBA,
            SWS_BILINEAR,
            nullptr,
            nullptr,
            nullptr
        );

        if (!scaleContext) {
            throw std::runtime_error("Could not create FFmpeg pixel format converter.");
        }

        std::uint8_t* destination[4] = {
            output.rgbaPixels.data(), nullptr, nullptr, nullptr
        };
        int destinationStride[4] = {
            width * 4, 0, 0, 0
        };

        const int rows = sws_scale(
            scaleContext,
            decodedFrame->data,
            decodedFrame->linesize,
            0,
            height,
            destination,
            destinationStride
        );

        if (rows != height) {
            throw std::runtime_error("FFmpeg did not convert the complete video frame.");
        }

        std::int64_t timestamp = decodedFrame->best_effort_timestamp;
        if (timestamp == AV_NOPTS_VALUE) {
            timestamp = decodedFrame->pts;
        }

        if (timestamp != AV_NOPTS_VALUE && timeBase.den != 0) {
            const double absoluteTime = static_cast<double>(timestamp) * av_q2d(timeBase);

            if (!hasTimestampOrigin) {
                timestampOrigin = absoluteTime;
                hasTimestampOrigin = true;
            }

            output.presentationTimeSeconds = std::max(0.0, absoluteTime - timestampOrigin);
            fallbackNextTime = output.presentationTimeSeconds + frameDurationFallback;
        } else {
            output.presentationTimeSeconds = fallbackNextTime;
            fallbackNextTime += frameDurationFallback;
        }
    }
};

VideoPlaybackManager::VideoPlaybackManager()
    : state(new DecoderState()) {
}

VideoPlaybackManager::~VideoPlaybackManager() = default;

void VideoPlaybackManager::open(const VideoAsset& video) {
    state->close();

    try {
        int result = avformat_open_input(&state->formatContext, video.path.c_str(), nullptr, nullptr);
        if (result < 0) {
            throw std::runtime_error("Could not open video '" + video.path + "': " + ffmpegErrorString(result));
        }

        result = avformat_find_stream_info(state->formatContext, nullptr);
        if (result < 0) {
            throw std::runtime_error("Could not read video stream information: " + ffmpegErrorString(result));
        }

        const AVCodec* decoder = nullptr;
        state->videoStreamIndex = av_find_best_stream(
            state->formatContext,
            AVMEDIA_TYPE_VIDEO,
            -1,
            -1,
            &decoder,
            0
        );

        if (state->videoStreamIndex < 0 || !decoder) {
            throw std::runtime_error("No usable video stream found in '" + video.path + "'.");
        }

        AVStream* stream = state->formatContext->streams[state->videoStreamIndex];
        state->timeBase = stream->time_base;

        state->codecContext = avcodec_alloc_context3(decoder);
        if (!state->codecContext) {
            throw std::runtime_error("Could not allocate video decoder context.");
        }

        result = avcodec_parameters_to_context(state->codecContext, stream->codecpar);
        if (result < 0) {
            throw std::runtime_error("Could not configure video decoder: " + ffmpegErrorString(result));
        }

        result = avcodec_open2(state->codecContext, decoder, nullptr);
        if (result < 0) {
            throw std::runtime_error("Could not start video decoder: " + ffmpegErrorString(result));
        }

        state->packet = av_packet_alloc();
        state->decodedFrame = av_frame_alloc();

        if (!state->packet || !state->decodedFrame) {
            throw std::runtime_error("Could not allocate FFmpeg frame or packet.");
        }

        AVRational frameRate = stream->avg_frame_rate;
        if (frameRate.num <= 0 || frameRate.den <= 0) {
            frameRate = stream->r_frame_rate;
        }

        if (frameRate.num > 0 && frameRate.den > 0) {
            state->frameDurationFallback =
                static_cast<double>(frameRate.den) / frameRate.num;
        }

        state->opened = true;
        state->playing = true;
    } catch (...) {
        state->close();
        throw;
    }
}

bool VideoPlaybackManager::update(double deltaTimeSeconds, DecodedVideoFrame& frameOut) {
    if (!state->opened || !state->playing || state->finished) {
        return false;
    }

    if (std::isfinite(deltaTimeSeconds) && deltaTimeSeconds > 0.0) {
        state->playbackTime += deltaTimeSeconds;
    }

    bool producedFrame = false;

    while (true) {
        if (!state->hasNextFrame) {
            if (!state->decodeNextFrame(state->nextFrame)) {
                state->finished = true;
                state->playing = false;
                return producedFrame;
            }

            state->hasNextFrame = true;
        }

        if (state->nextFrame.presentationTimeSeconds > state->playbackTime) {
            return producedFrame;
        }

        frameOut = std::move(state->nextFrame);
        state->hasNextFrame = false;
        producedFrame = true;
    }
}

void VideoPlaybackManager::pause() {
    state->playing = false;
}

void VideoPlaybackManager::resume() {
    if (state->opened && !state->finished) {
        state->playing = true;
    }
}

void VideoPlaybackManager::stop() {
    state->close();
}

bool VideoPlaybackManager::isPlaying() const {
    return state->playing;
}

bool VideoPlaybackManager::isFinished() const {
    return state->finished;
}