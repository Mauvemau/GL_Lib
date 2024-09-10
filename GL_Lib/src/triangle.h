#pragma once

#include "shape.h"

namespace gllib {

    class DLLExport Triangle: public Shape {
    private:

    public:
        Triangle(Transform transform);
        virtual ~Triangle() override;

        virtual void draw() override;
    };
}
