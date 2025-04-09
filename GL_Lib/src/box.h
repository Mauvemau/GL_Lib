#pragma once

#include "shape.h"

namespace gllib {
    class DLLExport Box : public Shape {
    private:
        Color color;

        void updateRenderData(Color color);
    public:
        Box(Vector3 translation, Vector3 rotation, Vector3 scale, Color color);
        Box(Transform transform, Color color);
        virtual ~Box() override;

        Color getColor();

        void setColor(Color color);

        virtual void draw() override;
    };
}
