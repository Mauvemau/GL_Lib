#include "triangle.h"

#include <iostream>

using namespace gllib;
using namespace std;

Triangle::Triangle() {
    const float triangleVertexData[] = {
        -0.5f, -0.5f, 0.0f, /* xyz */ 1.0f, 0.0f, 0.0f, 0.0f, /* rgba */ 0.0f, 0.0f, /* uv */
         0.0f,  0.5f, 0.0f, /* xyz */ 0.0f, 1.0f, 0.0f, 1.0f, /* rgba */ 1.0f, 0.0f, /* uv */
         0.5f, -0.5f, 0.0f, /* xyz */ 0.0f, 0.0f, 1.0f, 0.5f, /* rgba */ 0.0f, 1.0f, /* uv */
    };
    const int triangleIndex[] = {
        0, 1, 2
    };

    int vertexDataSize = sizeof(triangleVertexData) / sizeof(triangleVertexData[0]);
    int indexSize = sizeof(triangleIndex) / sizeof(triangleIndex[0]);

    initRenderData(triangleVertexData, vertexDataSize, triangleIndex, indexSize);
    cout << "Created triangle.\n";
}

Triangle::~Triangle() {
    cout << "Destroyed triangle.\n";
}

void Triangle::draw() {

    internalDraw();
}
