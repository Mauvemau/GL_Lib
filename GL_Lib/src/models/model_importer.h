#pragma once

#include "deps.h"
#include "mesh_group.h"

#include <assimp/scene.h>

namespace gllib {
    class DLLExport ModelImporter {
    private:
        static Mesh processMesh(aiMesh* mesh);
    public:
        static Mesh loadMesh(const std::string& path);
        static MeshGroup loadMeshGroup(const std::string& path);
    };
}