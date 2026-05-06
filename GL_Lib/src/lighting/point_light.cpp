#include "point_light.h"

#include <iostream>

using namespace gllib;
using namespace std;

PointLight::PointLight(Vector3 position) {
    this->position = position;
    constant = 1.0;
    linear = 0.09f;
    quadratic = 0.032f;

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

float PointLight::getConstant() const {
    return constant;
}

float PointLight::getLinear() const {
    return linear;
}

float PointLight::getQuadratic() const {
    return quadratic;
}

Vector3 PointLight::getPosition() const {
    return position;
}

void PointLight::setConstant(float constant) {
    this->constant = constant;
}

void PointLight::setLinear(float linear) {
    this->linear = linear;
}

void PointLight::setQuadratic(float quadratic) {
    this->quadratic = quadratic;
}

void PointLight::setPosition(Vector3 position) {
    this->position = position;
}

void PointLight::move(Vector3 direction) {
    position += direction;
}