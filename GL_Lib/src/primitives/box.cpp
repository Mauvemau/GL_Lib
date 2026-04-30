#include "box.h"

#include <iostream>
#include "../renderer.h"

using namespace gllib;
using namespace std;

Box::Box(Vector3 translation, Vector3 rotation, Vector3 scale, Color color) :
Shape(translation, rotation, scale) {
    this->color = color;
    updateRenderData(this->color);

    Vector3 ambient = Vector3(0.2f, 0.2f, 0.2f);
    Vector3 diffuse = Vector3(1.0f, 1.0f, 1.0f);
    Vector3 specular = Vector3(0.5f, 0.5f, 0.5f);
    material = Material(ambient, diffuse, specular, 80.0f);

    cout << "Created box.\n";
}

Box::Box(Vector3 translation, Vector3 rotation, Vector3 scale, Color color, Material mat) :
Shape(translation, rotation, scale) {
    this->color = color;
    updateRenderData(this->color);

    material = mat;

    cout << "Created box.\n";
}

Box::Box(Transform transform, Color color) :
Shape(transform) {
    this->color = color;
    updateRenderData(this->color);

    Vector3 ambient = Vector3(0.2f, 0.2f, 0.2f);
    Vector3 diffuse = Vector3(1.0f, 1.0f, 1.0f);
    Vector3 specular = Vector3(0.5f, 0.5f, 0.5f);
    material = Material(ambient, diffuse, specular, 80.0f);

    cout << "Created box.\n";
}

Box::Box(Transform transform, Color color, Material mat) :
Shape(transform) {
    this->color = color;
    updateRenderData(this->color);

    material = mat;

    cout << "Created box.\n";
}

Box::~Box() {
    cout << "Destroyed box.\n";
}

//

void Box::updateRenderData(Color color) {
    float boxVertexData[] = {
        // Front (+Z)
        1,1,1,  0,0,1,  color.r,color.g,color.b,color.a,  1,1,
        1,0,1,  0,0,1,  color.r,color.g,color.b,color.a,  1,0,
        0,0,1,  0,0,1,  color.r,color.g,color.b,color.a,  0,0,
        0,1,1,  0,0,1,  color.r,color.g,color.b,color.a,  0,1,
        // Back (-Z)
        1,1,0,  0,0,-1, color.r,color.g,color.b,color.a,  1,1,
        1,0,0,  0,0,-1, color.r,color.g,color.b,color.a,  1,0,
        0,0,0,  0,0,-1, color.r,color.g,color.b,color.a,  0,0,
        0,1,0,  0,0,-1, color.r,color.g,color.b,color.a,  0,1,
        // Right (+X)
        1,1,1,  1,0,0,  color.r,color.g,color.b,color.a,  1,1,
        1,1,0,  1,0,0,  color.r,color.g,color.b,color.a,  1,0,
        1,0,0,  1,0,0,  color.r,color.g,color.b,color.a,  0,0,
        1,0,1,  1,0,0,  color.r,color.g,color.b,color.a,  0,1,
        // Left (-X)
        0,1,1,  -1,0,0, color.r,color.g,color.b,color.a,  1,1,
        0,1,0,  -1,0,0, color.r,color.g,color.b,color.a,  1,0,
        0,0,0,  -1,0,0, color.r,color.g,color.b,color.a,  0,0,
        0,0,1,  -1,0,0, color.r,color.g,color.b,color.a,  0,1,
        // Top (+Y)
        1,1,1,  0,-1,0,  color.r,color.g,color.b,color.a,  1,1,
        0,1,1,  0,-1,0,  color.r,color.g,color.b,color.a,  0,1,
        0,1,0,  0,-1,0,  color.r,color.g,color.b,color.a,  0,0,
        1,1,0,  0,-1,0,  color.r,color.g,color.b,color.a,  1,0,
        // Bottom (-Y)
        1,0,1,  0,1,0, color.r,color.g,color.b,color.a,  1,1,
        0,0,1,  0,1,0, color.r,color.g,color.b,color.a,  0,1,
        0,0,0,  0,1,0, color.r,color.g,color.b,color.a,  0,0,
        1,0,0,  0,1,0, color.r,color.g,color.b,color.a,  1,0,
    };
    const int boxIndex[] = {
        0,1,2, 0,2,3,        // Front
        4,5,6, 4,6,7,        // Back
        8,9,10, 8,10,11,     // Right
        12,13,14, 12,14,15,  // Left
        16,18,17, 16,19,18,  // Top
        20,21,22, 20,22,23   // Bottom
    };

    alignVertex(boxVertexData, 24, 12);

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
    Renderer::setMaterial(*material);
    internalDraw();
}
