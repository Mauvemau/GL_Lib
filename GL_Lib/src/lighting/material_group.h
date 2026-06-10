#pragma once

#include <vector>
#include "material.h"

namespace gllib {

    class DLLExport MaterialGroup {
        private:
        std::vector<Material> materials;

        public:
        MaterialGroup(const std::vector<Material>& materials);
        ~MaterialGroup();

        std::vector<Material>& getMaterials();
    };

}