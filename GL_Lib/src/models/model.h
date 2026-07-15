#pragma once

#include "../lighting/material.h"
#include "shape_group.h"

#include "mesh.h"
#include "mesh_group.h"
#include "model_data.h"
#include "model_node.h"
#include "frustum.h"

namespace gllib {
    class DLLExport Model : public ShapeGroup {
        private:
        MeshGroup* meshGroup;
        ModelNode* rootNode;

        Color color{};

        int lastFrameDrawnCount = -1;
        int totalMeshesCount = 0;

        void updateRenderData(Color color, bool centerPivot);
        void renderNodeRecursive(ModelNode* node, const Frustum& frustum, int& drawnCounter);
        int countTotalMeshesRecursive(ModelNode* node) const;

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
