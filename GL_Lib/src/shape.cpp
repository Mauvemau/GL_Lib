#include "shape.h"

#include <iostream>

using namespace gllib;
using namespace std;

Shape::Shape(Transform transform) :
    Entity(transform){
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
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(glm::mat4(1.0f),
        glm::vec3(transform.position.x, transform.position.y, transform.position.z));

    if (transform.scale.x != 0 || transform.scale.y != 0 || transform.scale.z != 0)
        modelMatrix = glm::scale(modelMatrix, glm::vec3(transform.scale.x, transform.scale.y, 0.0f));

    if (transform.rotationQuat.x != 0 || transform.rotationQuat.y != 0 || transform.rotationQuat.z != 0)
    {
        modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.rotationQuat.x), glm::vec3(1.0, 0.0f, 0.0f));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.rotationQuat.y), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.rotationQuat.z), glm::vec3(0.0f, 0.0f, 1.0f));
    }
    Renderer::drawElements(renderData, indexSize);
}
