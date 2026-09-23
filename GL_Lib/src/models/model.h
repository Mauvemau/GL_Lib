#pragma once

#include "../lighting/material.h"
#include "shape_group.h"

#include "mesh.h"
#include "mesh_group.h"
#include "model_data.h"
#include "model_node.h"
#include "frustum.h"
#include "bsp_plane.h"

namespace gllib {
    class DLLExport Model : public ShapeGroup {
        private:
        MeshGroup* meshGroup;
        ModelNode* rootNode;

        Color color{};

        int lastFrameDrawnCount = -1;
        int totalMeshesCount = 0;

        std::vector<ModelNode*> bspNodes;
        std::vector<BSPPlane> bspPlanes;

        bool cullBSP = true;

        void updateRenderData(Color color, bool centerPivot);
        void renderNodeRecursive(ModelNode* node, const Frustum& frustum, int& drawnCounter);
        int countTotalMeshesRecursive(ModelNode* node) const;
        bool isBoxCulledByBSP(BoundingBox box, glm::vec3 cameraPosition) const;
        void collectBSPPlanes(ModelNode* node);
        void updateBSPPlanes();

        ModelNode* findNodeRecursive(ModelNode* node, int id);

        void drawBSPPlanesDebug();
        public:
        Model(ModelData& modelData, Transform transform, Color color);
        virtual ~Model() override;

        MeshGroup& getMeshGroup();
        Mesh& getMesh();

        Color getColor();

        bool getBSPCulling() const;

        void printBSPPlanes();

        void setMaterial(unsigned int index, const Material& mat);

        void setBSPCulling(bool enabled);

        ModelNode* findNode(int id);

        virtual void draw() override;
    };
}
