#pragma once

#include "deps.h"
#include <glm/glm.hpp>

namespace gllib {
    struct DLLExport BSPPlane {
        glm::vec3 point;
        glm::vec3 normal;
    };
}