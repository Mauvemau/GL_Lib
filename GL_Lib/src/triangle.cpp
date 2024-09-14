#include "triangle.h"

#include <iostream>

using namespace gllib;
using namespace std;

Triangle::Triangle(Transform transform) :
    Shape(transform){
    // Values for the vertices
    float triangleVertexData[] = {
        -1.0f, -1.0f, 0.0f, /* xyz */ 1.0f, 0.0f, 0.0f, 1.0f, /* rgba */ 0.0f, 0.0f, /* uv */
         1.0f, -1.0f, 0.0f, /* xyz */ 0.0f, 1.0f, 0.0f, 1.0f, /* rgba */ 1.0f, 0.0f, /* uv */
         0.0f,  1.0f, 0.0f, /* xyz */ 0.0f, 0.0f, 1.0f, 1.0f, /* rgba */ 0.0f, 1.0f, /* uv */
    };
    // The order in which the vertices are drawn
    const int triangleIndex[] = {
        0, 1, 2
    };

    int vertexCount = 3;
    int vertexStride = 9;

    // Calculate the centroid
    float centerX = 0.0f, centerY = 0.0f, centerZ = 0.0f;

    for (int i = 0; i < vertexCount; ++i) {
        centerX += triangleVertexData[i * vertexStride];
        centerY += triangleVertexData[i * vertexStride + 1];
        centerZ += triangleVertexData[i * vertexStride + 2];
    }

    centerX /= vertexCount;
    centerY /= vertexCount;
    centerZ /= vertexCount;

    // Adjust each vertex to center it around the origin
    for (int i = 0; i < vertexCount; ++i) {
        triangleVertexData[i * vertexStride] -= centerX;
        triangleVertexData[i * vertexStride + 1] -= centerY;
        triangleVertexData[i * vertexStride + 2] -= centerZ;
    }

    // Aquire the size of each buffer
    int vertexDataSize = sizeof(triangleVertexData) / sizeof(triangleVertexData[0]);
    int indexSize = sizeof(triangleIndex) / sizeof(triangleIndex[0]);

    // Initialize the render data on shape
    initRenderData(triangleVertexData, vertexDataSize, triangleIndex, indexSize);
    cout << "Created triangle.\n";
}

Triangle::~Triangle() {
    cout << "Destroyed triangle.\n";
}

void Triangle::draw() {

    // Shape has an internal draw function
    internalDraw();
}
