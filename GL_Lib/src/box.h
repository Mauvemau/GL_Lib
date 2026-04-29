#pragma once

#include "material.h"
#include "shape.h"
#include <optional>

namespace gllib {
    class DLLExport Box : public Shape {
    private:
        Color color{};
        std::optional<Material> material;

        void updateRenderData(Color color);
    public:
        Box(Vector3 translation, Vector3 rotation, Vector3 scale, Color color);
        Box(Vector3 translation, Vector3 rotation, Vector3 scale, Color color, Material mat);
        Box(Transform transform, Color color);
        Box(Transform transform, Color color, Material mat);
        virtual ~Box() override;

        Color getColor();

        void setColor(Color color);

        virtual void draw() override;
    };
}
