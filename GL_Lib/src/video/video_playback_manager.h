#pragma once

#include "video_asset.h"

#include <memory>
#include <vector>

namespace gllib {
    struct DLLExport DecodedVideoFrame {
        int width = 0, height = 0;
        double presentationTimeSeconds = 0.0;
        std::vector<unsigned char> rgbaPixels;
    };

    class DLLExport VideoPlaybackManager {
    private:
        struct DecoderState;
        std::unique_ptr<DecoderState> state;
    public:
        VideoPlaybackManager();
        ~VideoPlaybackManager();

        VideoPlaybackManager(const VideoPlaybackManager&) = delete;
        VideoPlaybackManager& operator=(const VideoPlaybackManager&) = delete;

        void open(const VideoAsset& video);

        bool update(double deltaTimeSeconds, DecodedVideoFrame& frameOut);

        void pause();
        void resume();
        void stop();

        bool isPlaying() const;
        bool isFinished() const;
    };
}