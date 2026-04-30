#include "light.h"
#include <iostream>

using namespace gllib;
using namespace std;

Light::Light() {
    this->color = {1.0f, 1.0f, 1.0f, 1.0f};
}

Light::Light(Color color) {
    this->color = color;

    cout << "Created light\n";
}

Light::~Light() {
    cout << "Destroyed light *evil god*\n";
}