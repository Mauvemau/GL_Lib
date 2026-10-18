#pragma once

#include "video_playback_manager.h"
#include "virtual_screen.h"
#include "../lighting/material.h"

#include <deque>
#include <memory>

namespace gllib {
    class DLLExport VirtualMediaPlayer {
        private:
        VideoPlaybackManager playbackManager;
        std::unique_ptr<VirtualScreen> screen;
        DecodedVideoFrame decodedFrame;
        std::deque<VideoAsset> queuedVideos;
        VideoAsset idleVideo;

        bool hasCurrentVideo = false;
        bool hasIdleVideo = false;
        bool currentIsIdle = false;
        bool paused = false;
        bool advancePending = false;

        void startVideo(const VideoAsset& video, bool isIdle);
        void startNextVideo();
        bool updateCurrentVideo(double deltaTimeSeconds);

        public:
        VirtualMediaPlayer() = default;
        ~VirtualMediaPlayer() = default;

        VirtualMediaPlayer(const VirtualMediaPlayer&) = delete;
        VirtualMediaPlayer& operator=(const VirtualMediaPlayer&) = delete;

        void play(const VideoAsset& video);
        void queue(const VideoAsset& video);
        bool skip();

        void setIdleVideo(const VideoAsset& video);
        void clearIdleVideo();

        bool update(double deltaTimeSeconds);
        void pause();
        void resume();
        void stop();

        bool isPlaying() const;
        bool isPlayingIdle() const;
        bool isFinished() const;

        Material getScreenMaterial() const;
    };
}