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
        virtual ~Animation() override;

        void setDurationInSecs(double durationInSecs);

        void update();
    };
}
