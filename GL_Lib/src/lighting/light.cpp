#include "light.h"
#include <iostream>

using namespace gllib;
using namespace std;

Light::Light() {
    this->color = {1.0f, 1.0f, 1.0f, 1.0f};
    active = true;

    cout << "Created light\n";
}

Light::Light(Color color) {
    this->color = color;
    active = true;

    cout << "Created light\n";
}

Light::~Light() {
    cout << "Destroyed light\n";
}

void Light::setActive(bool active) {
    this->active = active;
}

void Light::setColor(Color color) {
    this->color = color;
}

Color Light::getColor() const {
    return color;
}

bool Light::isActive() const {
    return active;
}