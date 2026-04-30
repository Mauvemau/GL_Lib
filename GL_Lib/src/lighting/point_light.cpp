#include "point_light.h"

#include <iostream>

using namespace gllib;
using namespace std;

PointLight::PointLight(Vector3 position) {
    this->position = position;
    constant = 1.0;
    linear = 0.7;
    quadratic = 1.8;

    cout << "Created Point Light!\n";
}

PointLight::PointLight(Vector3 position, float constant, float linear, float quadratic) {
    this->position = position;
    this->constant = constant;
    this->linear = linear;
    this->quadratic = quadratic;

    cout << "Created Point Light!\n";
}

PointLight::PointLight(Vector3 position, float constant, float linear, float quadratic, Color color) :
Light(color) {
    this->position = position;
    this->constant = constant;
    this->linear = linear;
    this->quadratic = quadratic;

    cout << "Created Point Light!\n";
}

PointLight::~PointLight() {
    cout << "Destroyed Point Light.\n";
}