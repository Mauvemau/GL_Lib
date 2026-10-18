#include "virtual_media_player.h"

#include <stdexcept>
#include <utility>

using namespace gllib;

void VirtualMediaPlayer::startVideo(const VideoAsset& video, bool isIdle) {
    hasCurrentVideo = false;
    currentIsIdle = false;
    paused = false;
    advancePending = false;
    decodedFrame = DecodedVideoFrame{};

    playbackManager.open(video);

    try {
        if (!screen) {
            screen.reset(new VirtualScreen(video.width, video.height));
        } else if (!screen->resize(video.width, video.height)) {
            throw std::runtime_error("Could not resize VirtualScreen.");
        }
    } catch (...) {
        playbackManager.stop();
        throw;
    }

    hasCurrentVideo = true;
    currentIsIdle = isIdle;
}

void VirtualMediaPlayer::startNextVideo() {
    if (!queuedVideos.empty()) {
        VideoAsset nextVideo = std::move(queuedVideos.front());
        queuedVideos.pop_front();
        startVideo(nextVideo, false);
    } else if (hasIdleVideo) {
        startVideo(idleVideo, true);
    } else {
        playbackManager.stop();
        hasCurrentVideo = false;
        currentIsIdle = false;
        paused = false;
    }
}

bool VirtualMediaPlayer::updateCurrentVideo(double deltaTimeSeconds) {
    if (!hasCurrentVideo || !playbackManager.update(deltaTimeSeconds, decodedFrame)) {
        return false;
    }

    if (decodedFrame.width != screen->getWidth() ||
        decodedFrame.height != screen->getHeight()) {
        if (!screen->resize(decodedFrame.width, decodedFrame.height)) {
            throw std::runtime_error("Could not resize VirtualScreen for decoded frame.");
        }
    }

    if (!screen->update(decodedFrame.rgbaPixels.data(), decodedFrame.rgbaPixels.size())) {
        throw std::runtime_error("Could not upload video frame to VirtualScreen.");
    }

    return true;
}

void VirtualMediaPlayer::play(const VideoAsset& video) {
    // Replaces the current video and preserves the pending queue.
    startVideo(video, false);
}

void VirtualMediaPlayer::queue(const VideoAsset& video) {
    queuedVideos.push_back(video);

    if (!hasCurrentVideo || currentIsIdle || playbackManager.isFinished()) {
        startNextVideo();
    }
}

bool VirtualMediaPlayer::skip() {
    if (queuedVideos.empty()) {
        if (!currentIsIdle) {
            stop();
        }
        return false;
    }

    const bool shouldRemainPaused = paused;
    advancePending = false;
    startNextVideo();

    updateCurrentVideo(0.0);

    if (shouldRemainPaused) {
        pause();
    }

    return true;
}

void VirtualMediaPlayer::setIdleVideo(const VideoAsset& video) {
    idleVideo = video;
    hasIdleVideo = true;

    if (currentIsIdle || (!hasCurrentVideo && queuedVideos.empty())) {
        startVideo(idleVideo, true);
    } else if (playbackManager.isFinished() && queuedVideos.empty()) {
        advancePending = true;
    }
}

void VirtualMediaPlayer::clearIdleVideo() {
    hasIdleVideo = false;

    if (!currentIsIdle) {
        return;
    }

    const bool shouldRemainPaused = paused;
    advancePending = false;

    if (!queuedVideos.empty()) {
        startNextVideo();
        updateCurrentVideo(0.0);

        if (shouldRemainPaused) {
            pause();
        }
    } else {
        playbackManager.stop();
        hasCurrentVideo = false;
        currentIsIdle = false;
        paused = false;
    }
}

bool VirtualMediaPlayer::update(double deltaTimeSeconds) {
    if (advancePending) {
        advancePending = false;
        startNextVideo();
        return updateCurrentVideo(0.0);
    }

    if (!hasCurrentVideo || paused) {
        return false;
    }

    const bool uploadedFrame = updateCurrentVideo(deltaTimeSeconds);

    if (playbackManager.isFinished()) {
        if (!queuedVideos.empty() || hasIdleVideo) {
            advancePending = true;
        } else {
            hasCurrentVideo = false;
            currentIsIdle = false;
        }
    }

    return uploadedFrame;
}

void VirtualMediaPlayer::pause() {
    if (hasCurrentVideo && playbackManager.isPlaying()) {
        playbackManager.pause();
        paused = true;
    }
}

void VirtualMediaPlayer::resume() {
    if (hasCurrentVideo && paused) {
        playbackManager.resume();
        paused = false;
    }
}

void VirtualMediaPlayer::stop() {
    playbackManager.stop();
    queuedVideos.clear();
    decodedFrame = DecodedVideoFrame{};

    hasCurrentVideo = false;
    currentIsIdle = false;
    paused = false;
    advancePending = false;

    if (hasIdleVideo) {
        startVideo(idleVideo, true);
    }
}

bool VirtualMediaPlayer::isPlaying() const {
    return hasCurrentVideo && playbackManager.isPlaying();
}

bool VirtualMediaPlayer::isPlayingIdle() const {
    return currentIsIdle && playbackManager.isPlaying();
}

bool VirtualMediaPlayer::isFinished() const {
    return !hasIdleVideo &&
           queuedVideos.empty() &&
           playbackManager.isFinished();
}

Material VirtualMediaPlayer::getScreenMaterial() const {
    if (!screen) {
        throw std::logic_error("Play or set an idle video before requesting the screen material.");
    }

    return Material(
        {1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f},
        {0.0f, 0.0f, 0.0f},
        0.0f,
        0.0f,
        screen->getTextureId()
    );
}