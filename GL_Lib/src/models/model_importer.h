#pragma once

#include "deps.h"
#include "mesh_group.h"

#include <assimp/scene.h>
#include <vector>

namespace gllib {
    class DLLExport ModelImporter {
    private:
        static Mesh processMesh(aiMesh* mesh);
        static void processNode(aiNode* node, const aiScene* scene, std::vector<Mesh>& meshes);
    public:
        static Mesh loadMesh(const std::string& path);
        static MeshGroup loadMeshGroup(const std::string &path);
    };
}