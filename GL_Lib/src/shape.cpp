#include "shape.h"

#include <iostream>

using namespace gllib;
using namespace std;

Shape::Shape() {
    cout << "Created shape.\n";
}

Shape::~Shape() {
    cout << "Destroyed shape.\n";
    Renderer::destroyRenderData(renderData);
}

// Protected

void Shape::initRenderData(const float vertexData[], GLsizei vertexDataSize, const int index[], GLsizei indexSize) {
    this->indexSize = indexSize;

    renderData = Renderer::createRenderData(vertexData, vertexDataSize, index, indexSize);
}

void Shape::internalDraw() {
    Renderer::drawElements(renderData, indexSize);
}
