#include "model.h"

#include <iostream>
#include "../renderer.h"

using namespace gllib;
using namespace std;

Model::Model(Mesh &mesh, Transform transform, Color color) :
ShapeGroup(transform) {
    this->color = color;

    ownedMeshGroup = MeshGroup({ mesh });
    meshGroup = &(*ownedMeshGroup);

    updateRenderData(this->color, true);
    cout << "Created model.\n";
}

Model::Model(MeshGroup& meshGroup, Transform transform, Color color) :
ShapeGroup(transform), meshGroup(&meshGroup) {

    this->color = color;

    updateRenderData(this->color, true);
    cout << "Created model.\n";
}

Model::Model(MeshGroup& meshGroup, MaterialGroup& materialGroup, Transform transform, Color color) :
ShapeGroup(transform), meshGroup(&meshGroup) {
    this->color = color;

    updateRenderData(this->color, true);

    vector<Material>& loadedMaterials = materialGroup.getMaterials();
    vector<Mesh>& meshes = meshGroup.getMeshes();

    for (size_t i = 0; i < meshes.size(); i++) {
        unsigned int matIdx = meshes[i].getMaterialIndex();

        if (matIdx < loadedMaterials.size()) {
            materials[i] = loadedMaterials[matIdx];
        }
    }

    cout << "Created model with automated, exact MaterialGroup mapping.\n";
}

Model::~Model() {
    cout << "Destroyed model.\n";
}

//

void Model::updateRenderData(Color color, bool centerPivot) {
    vector<Mesh>& meshes = meshGroup->getMeshes();
    vector<vector<float>> vertexGroups;

    for (Mesh& mesh : meshes) {
        vector<float> vertexData;
        vector<Vertex>& vertices = mesh.getVertices();

        for (Vertex& vertex : vertices) {
            // Position
            vertexData.push_back(vertex.position.x);
            vertexData.push_back(vertex.position.y);
            vertexData.push_back(vertex.position.z);
            // Normal
            vertexData.push_back(vertex.normal.x);
            vertexData.push_back(vertex.normal.y);
            vertexData.push_back(vertex.normal.z);
            // Color
            vertexData.push_back(color.r);
            vertexData.push_back(color.g);
            vertexData.push_back(color.b);
            vertexData.push_back(color.a);
            // UV
            vertexData.push_back(vertex.uv.x);
            vertexData.push_back(vertex.uv.y);
        }

        vertexGroups.push_back(vertexData);
    }

    Vector3 pivot(0,0,0);
    if (centerPivot) {
        pivot = calculateBoundsCenter(vertexGroups,12);
    }

    for (int i = 0; i < meshes.size(); i++) {
        Mesh& mesh = meshes[i];

        vector<float>& vertexData = vertexGroups[i];
        vector<int>& indices = mesh.getIndices();

        if (centerPivot) {
            pivotVertex(vertexData.data(), static_cast<int>(vertexData.size() / 12), 12, pivot);
        }

        addRenderData(vertexData.data(), static_cast<int>(vertexData.size()), indices.data(), static_cast<int>(indices.size()));
    }
}

Color Model::getColor() {
    return color;
}

void Model::setMaterial(unsigned int index, const Material& material) {
    if (index >= materials.size()) {
        return;
    }

    materials[index] = material;
}

MeshGroup& Model::getMeshGroup() {
    return *meshGroup;
}

Mesh& Model::getMesh() {
    return meshGroup->getMeshes()[0];
}

void Model::draw() {
    internalDraw();
}