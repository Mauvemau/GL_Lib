#pragma once

#include <string>
#include "model_node.h"
#include "mesh_group.h"
#include "../lighting/material_group.h"

namespace gllib {

    class DLLExport ModelData {
        private:
        ModelNode* rootNode;
        MeshGroup* meshGroup;
        MaterialGroup* materialGroup;

        public:
        ModelData(const std::string& path);
        ~ModelData();

        ModelNode* getRootNode() const;
        MeshGroup* getMeshGroup() const;
        MaterialGroup* getMaterialGroup() const;
    };

}