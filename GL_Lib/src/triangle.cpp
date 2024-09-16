#include "triangle.h"

#include <iostream>

using namespace gllib;
using namespace std;

Triangle::Triangle(Transform transform, Color color) :
    Shape(transform){
    this->color = color;
    // Values for the vertices
    float triangleVertexData[] = {
        -1.0f, -1.0f, 0.0f, /* xyz */ color.r, color.g, color.b, color.a, /* rgba */ 0.0f, 0.0f, /* uv */
         1.0f, -1.0f, 0.0f, /* xyz */ color.r, color.g, color.b, color.a, /* rgba */ 1.0f, 0.0f, /* uv */
         0.0f,  1.0f, 0.0f, /* xyz */ color.r, color.g, color.b, color.a, /* rgba */ 0.0f, 1.0f, /* uv */
    };
    // The order in which the vertices are drawn
    const int triangleIndex[] = {
        0, 1, 2
    };

    alignVertex(triangleVertexData, 3, 9);

    // Aquire the size of each buffer
    int vertexDataSize = sizeof(triangleVertexData) / sizeof(triangleVertexData[0]);
    int indexSize = sizeof(triangleIndex) / sizeof(triangleIndex[0]);

    // Initialize the render data on shape
    updateRenderData(triangleVertexData, vertexDataSize, triangleIndex, indexSize);
    cout << "Created triangle.\n";
}

Triangle::~Triangle() {
    cout << "Destroyed triangle.\n";
}

void Triangle::draw() {

    // Shape has an internal draw function
    internalDraw();
}
