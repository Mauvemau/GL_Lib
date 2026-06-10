#include "material_group.h"

#include <iostream>

using namespace gllib;
using namespace std;

MaterialGroup::MaterialGroup(const std::vector<Material>& materials) :
materials(materials) {
    cout << "Created Material Group!\n";
}

MaterialGroup::~MaterialGroup() {
    cout << "Destroyed Material Group\n";
}

std::vector<Material>& MaterialGroup::getMaterials() {
    return materials;
}