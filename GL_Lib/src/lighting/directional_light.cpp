#include "directional_light.h"

#include <iostream>

using namespace gllib;
using namespace std;

DirectionalLight::DirectionalLight(Vector3 direction) {
    this->direction = direction;

    cout << "Created Ambient Light!\n";
}

DirectionalLight::DirectionalLight(Vector3 direction, Color color) :
Light(color) {
    this->direction = direction;

    cout << "Created Ambient Light!\n";
}

DirectionalLight::~DirectionalLight() {
    cout << "Destroyed Ambient Light!\n";
}