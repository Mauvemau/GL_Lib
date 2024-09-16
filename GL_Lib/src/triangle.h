#pragma once

#include "shape.h"

namespace gllib {

    class DLLExport Triangle: public Shape {
    private:
        Color color;
    public:
        Triangle(Transform transform, Color color);
        virtual ~Triangle() override;

        virtual void draw() override;
    };
}
