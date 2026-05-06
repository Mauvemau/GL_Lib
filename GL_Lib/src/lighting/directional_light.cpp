#include "directional_light.h"

#include <iostream>

using namespace gllib;
using namespace std;

DirectionalLight::DirectionalLight() {
    direction = Vector3(-0.2f, -1.0f, -0.3f);
}

DirectionalLight::DirectionalLight(Vector3 direction) {
    this->direction = direction;

    cout << "Created Directional Light!\n";
}

DirectionalLight::DirectionalLight(Vector3 direction, Color color) :
Light(color) {
    this->direction = direction;

    cout << "Created Directional Light!\n";
}

DirectionalLight::~DirectionalLight() {
    cout << "Destroyed Directional Light!\n";
}


Vector3 DirectionalLight::getDirection() const {
    return direction;
}

void DirectionalLight::setDirection(Vector3 direction) {
    this->direction = direction;
}