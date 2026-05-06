#pragma once

#include "directional_light.h"
#include "point_light.h"

#include <vector>

namespace gllib {

    static constexpr int MAX_POINT_LIGHTS = 6;

    struct LightingData {
        bool hasDirectional = false;
        DirectionalLight* directionalLight;

        std::vector<PointLight*> pointLights;

        void SetDirectionalLight(DirectionalLight& light) {
            directionalLight = &light;
            hasDirectional = true;
        }

        void AddPointLight(PointLight& light) {
            pointLights.push_back(&light);
        }
    };

}
