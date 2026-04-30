#pragma once

#include "light.h"


namespace gllib {

    class DLLExport DirectionalLight : public Light {
    private:
        Vector3 direction;

    public:
        DirectionalLight(Vector3 direction);
        DirectionalLight(Vector3 direction, Color color);
        ~DirectionalLight() override;
    };

}