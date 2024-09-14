#pragma once

#include "renderer.h"
#include "entity.h"

namespace gllib {

    class DLLExport Shape : public Entity {
    private:
        RenderData renderData;
        unsigned int indexSize;

    protected:
        void alignVertex(float* vertexData, int vertexCount, int vertexStride);
        void initRenderData(const float vertexData[], int vertexDataSize, const int index[], int indexSize);
        void internalDraw();

    public:
        Shape(Transform transform);
        virtual ~Shape();

        virtual void draw() = 0;
    };
}
