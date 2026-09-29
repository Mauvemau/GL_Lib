#pragma once

#include "video_playback_manager.h"
#include "virtual_screen.h"
#include "../lighting/material.h"

#include <memory>

namespace gllib {
    class DLLExport VirtualMediaPlayer {
        private:
        VideoPlaybackManager playbackManager;
        std::unique_ptr<VirtualScreen> screen;
        DecodedVideoFrame decodedFrame;

        public:
        VirtualMediaPlayer() = default;
        ~VirtualMediaPlayer() = default;

        VirtualMediaPlayer(const VirtualMediaPlayer&) = delete;
        VirtualMediaPlayer& operator=(const VirtualMediaPlayer&) = delete;

        void load(const VideoAsset& video);
        bool update(double deltaTimeSeconds);

        void pause();
        void resume();
        void stop();

        bool isPlaying() const;
        bool isFinished() const;

        Material getScreenMaterial() const;
    };
}