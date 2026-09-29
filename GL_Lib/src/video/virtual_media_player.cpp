#include "virtual_media_player.h"

#include <stdexcept>
#include <iostream>

namespace gllib {

    void VirtualMediaPlayer::load(const VideoAsset& video) {
        stop();
        playbackManager.open(video);

        try {
            screen.reset(new VirtualScreen(video.width, video.height));
        } catch (...) {
            playbackManager.stop();
            throw;
        }
    }

    bool VirtualMediaPlayer::update(double deltaTimeSeconds) {
        bool hasFrame = playbackManager.update(deltaTimeSeconds, decodedFrame);

        if (!screen || !hasFrame) {
            return false;
        }

        bool uploaded = screen->update(decodedFrame.rgbaPixels.data(), decodedFrame.rgbaPixels.size());
        if (!uploaded) {
            throw std::runtime_error("Could not upload video frame to VirtualScreen.");
        }

        return true;
    }

    void VirtualMediaPlayer::pause() {
        playbackManager.pause();
    }

    void VirtualMediaPlayer::resume() {
        playbackManager.resume();
    }

    void VirtualMediaPlayer::stop() {
        playbackManager.stop();
        screen.reset();
        decodedFrame = DecodedVideoFrame{};
    }

    bool VirtualMediaPlayer::isPlaying() const {
        return playbackManager.isPlaying();
    }

    bool VirtualMediaPlayer::isFinished() const {
        return playbackManager.isFinished();
    }

    Material VirtualMediaPlayer::getScreenMaterial() const {
        if (!screen) {
            throw std::logic_error("Load a video before requesting its screen material.");
        }

        return Material(screen->getTextureId());
    }

}