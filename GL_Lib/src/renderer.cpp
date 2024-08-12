#include "renderer.h"

#include <iostream>

using namespace gllib;
using namespace std;

void Renderer::setUpVertexAttributes() {
    // position attribute
    // Pointer id 0, length is 3 floats (xyz), each line is 9 floats long in total (xyz,rgba,uv), value begins at position 0 on this line. 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    // Pointer id 1, length is 4 floats (rgba), value begins at position 3 on this line (after xyz). 
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // uv attribute
    // Pointer id 2, length is 2 floats (uv), value begins at position 7 on this line (after xyzrgba).
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

unsigned int Renderer::createVertexArrayObject() {
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    return VAO;
}

unsigned int Renderer::createVertexBufferObject(const float vertexData[], size_t bufferSize) {
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, bufferSize, vertexData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return VBO;
}

unsigned int Renderer::createElementBufferObject(const int index[], size_t bufferSize) {
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferSize, index, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    return EBO;
}

RenderData Renderer::createRenderData(const float vertexData[], size_t vertexDataSize, const int index[], size_t indexSize) {
    RenderData rData;

    // VAO will store the attribute pointers of our buffer.
    rData.VAO = createVertexArrayObject();
    glBindVertexArray(rData.VAO); // We need to bind VAO before setting up the attributes.

    // VBO will store the data of the vertices such as; position, color, alpha, texture coords, etc.
    rData.VBO = createVertexBufferObject(vertexData, vertexDataSize * sizeof(float));
    // EBO will store the indices, this will define the order in which we're drawing.
    rData.EBO = createElementBufferObject(index, indexSize * sizeof(int));

    // Before we set the attributes we need to have the VAO binded because that's where the pointers to these attributes will be saved.
    // We also have to bind VBO because we need to tell OpenGL which buffer we'll be using.
    glBindBuffer(GL_ARRAY_BUFFER, rData.VBO);
    setUpVertexAttributes();
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // We need to specify the color blending to use the alpha channel.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    cout << "Render data created! VAO: " << rData.VAO << ", VBO: " << rData.VBO << ", EBO: " << rData.EBO << ".\n";

    // Unbinding after finishing simply for the sake of better binding understanding.
    glBindVertexArray(0);
    return rData;
}

void Renderer::destroyRenderData(RenderData rData) {
    glDeleteBuffers(1, &rData.EBO);
    glDeleteBuffers(1, &rData.VBO);
    glDeleteVertexArrays(1, &rData.VAO);

    printf("Render data destroyed.\n");
}

void Renderer::drawElements(RenderData rData, size_t indexSize) {
    glBindVertexArray(rData.VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rData.EBO);

    glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}