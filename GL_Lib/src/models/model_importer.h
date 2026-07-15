#pragma once

#include "deps.h"
#include "mesh_group.h"
#include "model_node.h"
#include "../lighting/material_group.h"

#include <assimp/scene.h>
#include <vector>

namespace gllib {
    struct ImportedModelParts {
        ModelNode* rootNode;
        std::vector<Mesh> meshes;
    };

    class DLLExport ModelImporter {
    private:
        static Mesh processMesh(aiMesh* mesh);
        static void processNode(aiNode* node, const aiScene* scene, std::vector<Mesh>& meshes);
        static Material processMaterial(aiMaterial* material, const std::string& directory);

        static ModelNode* processNodeSynchronized(aiNode* node, const aiScene* scene, std::vector<Mesh>& outMeshes, int& meshCounter, int& nodeCounter);
        static glm::mat4 aiMatrixToGlm(const aiMatrix4x4& from);
        static BoundingBox calculateLocalBounds(Mesh& mesh);

    public:
        static ImportedModelParts loadModelParts(const std::string& path);

        static MeshGroup loadMeshGroup(const std::string &path);
        static MaterialGroup loadMaterialGroup(const std::string& path);
    };
}