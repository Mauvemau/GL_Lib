#pragma once
#include <map>
#include <string>

#include "transform.h"
#include "sprite.h"

namespace gllib
{
    class DLLExport Animation : public Sprite {
    private:
        double durationInSecs;
        double elapsedTime;

    public:
        Animation(Vector3 translation, Vector3 rotation, Vector3 scale, Color color);
        Animation(Transform transform, Color color);
        Animation(Sprite sprite);
        virtual ~Animation() override;

        void addFramesFromAtlas(std::string path, int startX, int startY, int frameWidth,
                                int frameHeight, int columns, int rows, bool transparent);
        void addFrames(unsigned int textureID, int frameWidth, int frameHeight, int columns, int rows);
        void setDurationInSecs(double durationInSecs);

        void update();
    };
}
