#pragma once

#include "directional_light.h"
#include "point_light.h"
#include "spot_light.h"

#include <vector>
#include <algorithm>

namespace gllib {

    static constexpr int MAX_POINT_LIGHTS = 6;
    static constexpr int MAX_SPOT_LIGHTS = 6;

    struct LightingData {
        bool hasDirectional = false;
        DirectionalLight* directionalLight = nullptr;

        std::vector<PointLight*> pointLights;
        std::vector<SpotLight*> spotLights;

        void SetDirectionalLight(DirectionalLight& light) {
            directionalLight = &light;
            hasDirectional = true;
        }

        void RemoveDirectionalLight() {
            hasDirectional = false;
            directionalLight = nullptr;
        }

        void AddPointLight(PointLight& light) {
            if (pointLights.size() >= MAX_POINT_LIGHTS) return;
            pointLights.push_back(&light);
        }

        void RemovePointLight(PointLight& light) {
            auto it = std::find(pointLights.begin(), pointLights.end(), &light);
            if (it != pointLights.end()) {
                pointLights.erase(it);
            }
        }

        void AddSpotLight(SpotLight& light) {
            if (spotLights.size() >= MAX_SPOT_LIGHTS) return;
            spotLights.push_back(&light);
        }

        void RemoveSpotLight(SpotLight& light) {
            auto it = std::find(spotLights.begin(), spotLights.end(), &light);
            if (it != spotLights.end()) {
                spotLights.erase(it);
            }
        }
    };

}
