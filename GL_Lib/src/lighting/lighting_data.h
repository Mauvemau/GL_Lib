#pragma once

#include "directional_light.h"
#include "point_light.h"

#include <vector>

namespace gllib {

    struct LightingData {
        DirectionalLight* directionalLight = nullptr;
        std::vector<PointLight> pointLights;
    };

}
