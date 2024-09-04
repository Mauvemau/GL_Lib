#include "shape.h"

#include <iostream>

using namespace gllib;
using namespace std;

Shape::Shape() {
    cout << "Created shape.\n";
}

Shape::~Shape() {
    cout << "Destroyed shape.\n";
    // Destroy the render data to free up vram
    Renderer::destroyRenderData(renderData);
}

// Protected

void Shape::initRenderData(const float vertexData[], GLsizei vertexDataSize, const int index[], GLsizei indexSize) {
    // Update the size of the index (The size will depend on the shape drawn)
    this->indexSize = indexSize;

    // Create the render data for the buffers
    renderData = Renderer::createRenderData(vertexData, vertexDataSize, index, indexSize);
}

void Shape::internalDraw() {
    Renderer::drawElements(renderData, indexSize);
}
