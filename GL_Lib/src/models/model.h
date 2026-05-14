#pragma once

#include <optional>

#include "../lighting/material.h"
#include "shape_group.h"

#include "mesh.h"
#include "mesh_group.h"

namespace gllib {
    class DLLExport Model : public ShapeGroup {
    private:
        std::optional<MeshGroup> ownedMeshGroup;
        MeshGroup* meshGroup;

        Color color{};

        std::optional<Material> material;

        void updateRenderData(Color color, bool centerPivot);

    public:
        Model(Mesh &mesh,Transform transform,Color color);
        Model(Mesh &mesh,Transform transform,Color color, Material mat);
        Model(MeshGroup &meshGroup,Transform transform, Color color);
        Model(MeshGroup &meshGroup, Transform transform, Color color, Material mat);
        virtual ~Model() override;

        MeshGroup& getMeshGroup();
        Mesh& getMesh();

        Color getColor();

        void setColor(Color color);

        virtual void draw() override;
    };
}
