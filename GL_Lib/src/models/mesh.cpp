#include "mesh.h"

#include <iostream>

using namespace gllib;
using namespace std;

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<int>& indices) {
    this->vertices = vertices;
    this->indices = indices;

    //cout << "Created mesh.\n";
}

Mesh::~Mesh() {
    //cout << "Destroyed mesh.\n";
}

std::vector<Vertex>& Mesh::getVertices() {
    return vertices;
}

std::vector<int>& Mesh::getIndices() {
    return indices;
}
