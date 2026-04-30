#pragma once

#include "light.h"

namespace gllib {

    class DLLExport PointLight : public Light {
    private:
        Vector3 position;

        float constant;
        float linear;
        float quadratic;
    public:
        PointLight(Vector3 position);
        PointLight(Vector3 position, float constant, float linear, float quadratic);
        PointLight(Vector3 position, float constant, float linear, float quadratic, Color color);
        ~PointLight() override;
    };

}