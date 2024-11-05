#include "animation.h"

#include <iostream>

#include "lib_time.h"

using namespace std;

namespace gllib {
    Animation::Animation(Vector3 translation, Vector3 rotation, Vector3 scale, Color color) : 
        Sprite(translation, rotation, scale, color) {
        durationInSecs = .0f;
        elapsedTime = .0f;

        cout << "Created animation.\n";
    }

    Animation::Animation(Transform transform, Color color) : 
        Sprite(transform, color) {
        durationInSecs = .0f;
        elapsedTime = .0f;

        cout << "Created animation.\n";
    }

    Animation::~Animation() {
        cout << "Destroyed animation.\n";
    }

    // public

    void Animation::setDurationInSecs(double durationInSecs) {
        this->durationInSecs = durationInSecs;
    }

    void Animation::update() {
        elapsedTime += LibTime::getDeltaTime();

        double normalizedTime = fmod(elapsedTime, durationInSecs);
        int totalFrames = getFrameCount();

        if (totalFrames < 1)
            return;

        int framesPerSecond = static_cast<int>(totalFrames / durationInSecs);
        int currentFrame = static_cast<int>(normalizedTime * framesPerSecond) % totalFrames;

        setCurrentFrame(currentFrame);
    }

}