#pragma once

#include "shape.h"

namespace gllib {

    class DLLExport Triangle: public Shape {
    private:

    public:
        Triangle();
        virtual ~Triangle() override;

        virtual void draw() override;
    };
}
