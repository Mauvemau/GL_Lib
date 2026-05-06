#include "spot_light.h"

#include <iostream>

using namespace gllib;
using namespace std;

SpotLight::SpotLight(Vector3 position, Vector3 direction) {
    this->position = position;
    this->direction = direction;

    setCutOff(12.5f);
    setOuterCutOff(15.0f);

    constant = 1.0f;
    linear = 0.09f;
    quadratic = 0.032f;

    cout << "Created Spot Light!\n";
}

SpotLight::SpotLight(Vector3 position, Vector3 direction, Color color) :
Light(color) {
    this->position = position;
    this->direction = direction;

    setCutOff(12.5f);
    setOuterCutOff(15.0f);

    constant = 1.0f;
    linear = 0.09f;
    quadratic = 0.032f;

    cout << "Created Spot Light!\n";
}

SpotLight::SpotLight(Vector3 position, Vector3 direction, float cutOff, float outerCutOff, float constant, float linear, float quadratic) {
    this->position = position;
    this->direction = direction;

    setCutOff(cutOff);
    setOuterCutOff(outerCutOff);

    this->constant = constant;
    this->linear = linear;
    this->quadratic = quadratic;

    cout << "Created Spot Light!\n";
}

SpotLight::SpotLight(Vector3 position, Vector3 direction, float cutOff, float outerCutOff, float constant, float linear, float quadratic, Color color) :
Light(color) {
    this->position = position;
    this->direction = direction;

    setCutOff(cutOff);
    setOuterCutOff(outerCutOff);

    this->constant = constant;
    this->linear = linear;
    this->quadratic = quadratic;

    cout << "Created Spot Light!\n";
}

SpotLight::~SpotLight() {
    cout << "Destroyed Spot Light\n";
}

float SpotLight::getCutOffCos() const {
    return cutOff;
}

float SpotLight::getOuterCutOffCos() const {
    return outerCutOff;
}

float SpotLight::getConstant() const {
    return constant;
}

float SpotLight::getLinear() const {
    return linear;
}

float SpotLight::getQuadratic() const {
    return quadratic;
}

Vector3 SpotLight::getPosition() const {
    return position;
}

Vector3 SpotLight::getDirection() const {
    return direction;
}

void SpotLight::setCutOff(float degrees) {
    cutOff = std::cos(glm::radians(degrees));
}

void SpotLight::setOuterCutOff(float degrees) {
    outerCutOff = std::cos(glm::radians(degrees));
}

void SpotLight::setConstant(float constant) {
    this->constant = constant;
}

void SpotLight::setLinear(float linear) {
    this->linear = linear;
}

void SpotLight::setQuadratic(float quadratic) {
    this->quadratic = quadratic;
}

void SpotLight::setPosition(Vector3 position) {
    this->position = position;
}
void SpotLight::setDirection(Vector3 direction) {
    this->direction = direction;
}

void SpotLight::move(Vector3 direction) {
    this->position += direction;
}