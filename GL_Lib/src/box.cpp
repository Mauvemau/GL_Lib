#include "box.h"

#include <iostream>

using namespace gllib;
using namespace std;

Box::Box(Vector3 translation, Vector3 rotation, Vector3 scale, Color color) :
Shape(translation, rotation, scale) {
    this->color = color;
    updateRenderData(this->color);
    cout << "Created box.\n";
}

Box::Box(Transform transform, Color color) :
Shape(transform) {
    this->color = color;
    updateRenderData(this->color);
    cout << "Created box.\n";
}

Box::~Box() {
    cout << "Destroyed box.\n";
}

//

void Box::updateRenderData(Color color) {
    float boxVertexData[] = {
        // Front face
        1.0f, 1.0f, 1.0f, color.r, color.g, color.b, color.a, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, color.r, color.g, color.b, color.a, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f, color.r, color.g, color.b, color.a, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f, color.r, color.g, color.b, color.a, 0.0f, 1.0f,
        // Back face
        1.0f, 1.0f, 0.0f, color.r, color.g, color.b, color.a, 1.0f, 1.0f,
        1.0f, 0.0f, 0.0f, color.r, color.g, color.b, color.a, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, color.r, color.g, color.b, color.a, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, color.r, color.g, color.b, color.a, 0.0f, 1.0f
    };
    const int boxIndex[] = {
        // Front
        0, 1, 3, 1, 2, 3,
        // Right
        0, 4, 1, 1, 4, 5,
        // Back
        4, 7, 5, 5, 7, 6,
        // Left
        7, 3, 6, 6, 3, 2,
        // Top
        0, 3, 4, 4, 3, 7,
        // Bottom
        1, 5, 2, 2, 5, 6
    };

    alignVertex(boxVertexData, 8, 9);

    int vertexDataSize = sizeof(boxVertexData) / sizeof(boxVertexData[0]);
    int indexSize = sizeof(boxIndex) / sizeof(boxIndex[0]);

    setRenderData(boxVertexData, vertexDataSize, boxIndex, indexSize);
}

// Public

Color Box::getColor() {
    return color;
}

void Box::setColor(Color color) {
    this->color = color;
    updateRenderData(this->color);
}

void Box::draw() {
    internalDraw();
}
