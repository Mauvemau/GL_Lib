#pragma once

#include "renderer.h"

namespace gllib {

    class DLLExport Shape {
    private:
        RenderData renderData;
        unsigned int indexSize;

    protected:
        void initRenderData(const float vertexData[], int vertexDataSize, const int index[], int indexSize);
        void internalDraw();

    public:
        Shape();
        virtual ~Shape();

        virtual void draw() = 0;
    };
}
