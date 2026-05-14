#pragma once

#include <vector>

#include "../renderer.h"
#include "../entity.h"

namespace gllib {

    class DLLExport ShapeGroup : public Entity {
        private:
        std::vector<RenderData> renderDatas;
        std::vector<unsigned int> indexSizes;

        protected:
        void addRenderData(const float vertexData[], int vertexDataSize, const int index[], int indexSize);
        void pivotVertex(float* vertexData, int vertexCount, int vertexStride, Vector3 pivot);
        Vector3 calculateBoundsCenter(const std::vector<std::vector<float>>& vertexGroups, int vertexStride);
        void internalDraw();

        public:
        ShapeGroup(Vector3 translation, Vector3 rotation, Vector3 scale);
        ShapeGroup(Transform transform);
        virtual ~ShapeGroup();

        virtual void draw() = 0;
    };

}