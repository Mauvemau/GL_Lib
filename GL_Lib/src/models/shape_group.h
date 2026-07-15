#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "../renderer.h"
#include "../entity.h"

namespace gllib {

    class DLLExport ShapeGroup : public Entity {
        private:
        std::vector<RenderData> renderDatas;
        std::vector<unsigned int> indexSizes;

        Material getDefaultMaterial();

        protected:
        std::vector<Material> materials;

        void addRenderData(const float vertexData[], int vertexDataSize, const int index[], int indexSize);
        void pivotVertex(float* vertexData, int vertexCount, int vertexStride, Vector3 pivot);
        Vector3 calculateBoundsCenter(const std::vector<std::vector<float>>& vertexGroups, int vertexStride);
        void internalDraw();

        void drawSubMesh(int meshIndex, const glm::mat4& transformMatrix);

        const std::vector<RenderData>& getRenderDatas() const { return renderDatas; }
        const std::vector<unsigned int>& getIndexSizes() const { return indexSizes; }

        public:
        ShapeGroup(Vector3 translation, Vector3 rotation, Vector3 scale);
        ShapeGroup(Transform transform);
        virtual ~ShapeGroup();

        virtual void draw() = 0;
    };

}