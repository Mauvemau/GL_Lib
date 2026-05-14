#include "mesh_group.h"

#include <iostream>

using namespace gllib;
using namespace std;

MeshGroup::MeshGroup(const std::vector<Mesh>& meshes) {
    this->meshes = meshes;

    cout << "Created mesh group.\n";
}

MeshGroup::~MeshGroup() {
    cout << "Destroyed mesh group.\n";
}

std::vector<Mesh>& MeshGroup::getMeshes() {
    return meshes;
}