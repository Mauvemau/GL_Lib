#pragma once

#include "directional_light.h"
#include "point_light.h"
#include "spot_light.h"

#include <vector>

namespace gllib {

    static constexpr int MAX_POINT_LIGHTS = 6;
    static constexpr int MAX_SPOT_LIGHTS = 6;

    struct LightingData {
        bool hasDirectional = false;
        DirectionalLight* directionalLight;

        std::vector<PointLight*> pointLights;
        std::vector<SpotLight*> spotLights;

        void SetDirectionalLight(DirectionalLight& light) {
            directionalLight = &light;
            hasDirectional = true;
        }

        void AddPointLight(PointLight& light) {
            if (pointLights.size() >= MAX_POINT_LIGHTS) return;
            pointLights.push_back(&light);
        }

        void AddSpotLight(SpotLight& light) {
            if (spotLights.size() >= MAX_SPOT_LIGHTS) return;
            spotLights.push_back(&light);
        }
    };

}
