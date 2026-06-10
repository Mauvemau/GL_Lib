#include "mesh.h"

#include <iostream>

using namespace gllib;
using namespace std;

Mesh::Mesh(const vector<Vertex>& vertices, const vector<int>& indices, unsigned int materialIndex)
    : vertices(vertices), indices(indices), materialIndex(materialIndex) {}

Mesh::~Mesh() {}

std::vector<Vertex>& Mesh::getVertices() {
    return vertices;
}

std::vector<int>& Mesh::getIndices() {
    return indices;
}

unsigned int Mesh::getMaterialIndex() const {
    return materialIndex;
}
