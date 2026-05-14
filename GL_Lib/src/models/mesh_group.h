#pragma once

#include <vector>

#include "mesh.h"

namespace gllib {

    class DLLExport MeshGroup {
        private:
        std::vector<Mesh> meshes;

        public:
        MeshGroup(const std::vector<Mesh>& meshes);
        ~MeshGroup();

        std::vector<Mesh>& getMeshes();
    };

}