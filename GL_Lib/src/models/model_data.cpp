#include "model_data.h"

#include "loader.h"
#include "model_importer.h"
#include <iostream>

using namespace std;

namespace gllib {

    ModelData::ModelData(const std::string& path) : rootNode(nullptr), meshGroup(nullptr), materialGroup(nullptr) {
        if (!Loader::fileExists(path)) {
            cout << "[ERROR] ModelData -> The file doesn't exist in the specified path: " << path << "\n";
            return;
        }

        materialGroup = new MaterialGroup(ModelImporter::loadMaterialGroup(path));

        ImportedModelParts parts = ModelImporter::loadModelParts(path);
        rootNode = parts.rootNode;
        meshGroup = new MeshGroup(parts.meshes);
        cout << "Created Model Data!\n";
    }

    ModelData::~ModelData() {
        if (rootNode != nullptr) {
            delete rootNode;
            rootNode = nullptr;
        }
        if (meshGroup != nullptr) {
            delete meshGroup;
            meshGroup = nullptr;
        }
        if (materialGroup != nullptr) {
            delete materialGroup;
            materialGroup = nullptr;
        }
        cout << "Destroyed Model Data\n";
    }

    ModelNode* ModelData::getRootNode() const {
        return rootNode;
    }

    MeshGroup* ModelData::getMeshGroup() const {
        return meshGroup;
    }

    MaterialGroup* ModelData::getMaterialGroup() const {
        return materialGroup;
    }

}