#include "video_loader.h"

#include <stdexcept>
#include <string>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/error.h>
#include <libavutil/rational.h>
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

double rationalToDouble(AVRational value) {
    if (value.num <= 0 || value.den <= 0) {
        return 0.0;
    }

    return static_cast<double>(value.num) / static_cast<double>(value.den);
}

} // namespace

VideoAsset VideoLoader::load(const std::string& path) {
    AVFormatContext* formatContext = nullptr;
    int result = avformat_open_input(&formatContext, path.c_str(), nullptr, nullptr);

    if (result < 0) {
        throw std::runtime_error("Could not open video '" + path + "': " + ffmpegErrorString(result));
    }

    struct FormatContextGuard {
        AVFormatContext** context;

        ~FormatContextGuard() {
            if (context && *context) {
                avformat_close_input(context);
            }
        }
    } guard{&formatContext};

    result = avformat_find_stream_info(formatContext, nullptr);
    if (result < 0) {
        throw std::runtime_error("Could not read video stream information from '" + path + "': " + ffmpegErrorString(result));
    }

    const AVCodec* decoder = nullptr;
    const int streamIndex = av_find_best_stream(formatContext, AVMEDIA_TYPE_VIDEO, -1, -1, &decoder, 0);

    if (streamIndex < 0) {
        throw std::runtime_error("No usable video stream found in '" + path + "': " + ffmpegErrorString(streamIndex));
    }
    AVStream* stream = formatContext->streams[streamIndex];
    AVCodecParameters* parameters = stream->codecpar;

    if (!decoder || !parameters) {
        throw std::runtime_error("Could not find a decoder for the video stream in '" + path + "'");
    }

    VideoAsset asset;
    asset.path = path;
    asset.width = parameters->width;
    asset.height = parameters->height;
    asset.codecName = decoder->name ? decoder->name : "";

    AVRational frameRate = stream->avg_frame_rate;
    if (frameRate.num <= 0 || frameRate.den <= 0) {
        frameRate = stream->r_frame_rate;
    }

    if (frameRate.num > 0 && frameRate.den > 0) {
        asset.frameRateNumerator = frameRate.num;
        asset.frameRateDenominator = frameRate.den;
    }

    if (stream->duration != AV_NOPTS_VALUE && stream->duration > 0 && stream->time_base.den > 0) {
        asset.durationInSeconds = static_cast<double>(stream->duration) * av_q2d(stream->time_base);
    }
    else if (formatContext->duration != AV_NOPTS_VALUE && formatContext->duration > 0) {
        asset.durationInSeconds = static_cast<double>(formatContext->duration) / static_cast<double>(AV_TIME_BASE);
    }

    return asset;
}