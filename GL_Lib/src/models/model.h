#pragma once

#include <optional>
#include <map>

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

        void updateRenderData(Color color, bool centerPivot);

    public:
        Model(Mesh &mesh,Transform transform,Color color);
        Model(MeshGroup &meshGroup,Transform transform, Color color);
        virtual ~Model() override;

        MeshGroup& getMeshGroup();
        Mesh& getMesh();

        Color getColor();

        void setMaterial(unsigned int index, const Material& mat);

        virtual void draw() override;
    };
}
