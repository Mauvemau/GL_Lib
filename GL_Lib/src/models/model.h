#pragma once

#include <optional>
#include <map>
#include <string>

#include "../lighting/material.h"
#include "shape_group.h"

#include "mesh.h"
#include "mesh_group.h"
#include "model_data.h"
#include "model_node.h"
#include "../lighting/material_group.h"

namespace gllib {
    class DLLExport Model : public ShapeGroup {
        private:
        MeshGroup* meshGroup;
        ModelNode* rootNode;

        Color color{};

        void updateRenderData(Color color, bool centerPivot);

        void renderNodeRecursive(ModelNode* node);
        ModelNode* findNodeRecursive(ModelNode* node, int id);

        public:
        Model(ModelData& modelData, Transform transform, Color color);
        virtual ~Model() override;

        MeshGroup& getMeshGroup();
        Mesh& getMesh();

        Color getColor();

        void setMaterial(unsigned int index, const Material& mat);

        ModelNode* findNode(int id);

        virtual void draw() override;
    };
}
