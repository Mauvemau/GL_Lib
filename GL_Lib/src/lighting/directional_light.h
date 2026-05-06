#pragma once

#include "light.h"

namespace gllib {

    class DLLExport DirectionalLight : public Light {
    private:
        Vector3 direction;

    public:
        DirectionalLight();
        DirectionalLight(Vector3 direction);
        DirectionalLight(Vector3 direction, Color color);
        ~DirectionalLight() override;

        Vector3 getDirection() const;

        void setDirection(Vector3 direction);
    };

}