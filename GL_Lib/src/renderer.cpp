#include "renderer.h"

#include <iostream>

#include "transform.h"

using namespace gllib;
using namespace std;

glm::mat4 Renderer::projMatrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
glm::mat4 Renderer::viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
glm::mat4 Renderer::modelMatrix = glm::mat4(1.0f);

glm::vec3 Renderer::cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 Renderer::lightPos = glm::vec3(3.0f, 0.0f, 3.0f);

void Renderer::setUpVertexAttributes() {
    // position attribute
    // Pointer id 0, length is 3 floats (xyz), each line is 12 floats long in total (xyz,nxnynz,rgba,uv), value begins at position 0 on this line.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);
    // normal attribute
    // Pointed id 1, length is 3 floats (nx, ny, nz), value begins at position 3 on this line (after xyz).
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // color attribute
    // Pointer id 2, length is 4 floats (rgba), value begins at position 6 on this line (after xyznxnynz).
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(float), reinterpret_cast<void *>(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // uv attribute
    // Pointer id 3, length is 2 floats (uv), value begins at position 10 on this line (after xyznxnynzrgba).
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(float), reinterpret_cast<void *>(10 * sizeof(float)));
    glEnableVertexAttribArray(3);
}

void Renderer::setDefaultMaterial() {
    GLint prog = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prog);

    int materialAmbientLoc = glGetUniformLocation(prog, "u_material.ambient");
    int materialDiffuseLoc = glGetUniformLocation(prog, "u_material.diffuse");
    int materialSpecularLoc = glGetUniformLocation(prog, "u_material.specular");
    int materialShininessLoc = glGetUniformLocation(prog, "u_material.shininess");
    glUniform3f(materialAmbientLoc, 0.1f, 0.1f, 0.1f);
    glUniform3f(materialDiffuseLoc, 1.0f, 1.0f, 1.0f);
    glUniform3f(materialSpecularLoc, 0.5f, 0.5f, 0.5f);
    glUniform1f(materialShininessLoc, 80.0f);
}

void Renderer::setUpMVP() {
    // TRS
    // the mpv matrix is calculated multiplying p*v*m
    glm::mat4 mvp = projMatrix * viewMatrix * modelMatrix;
    GLint prog = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prog);
    int mvpLocation = glGetUniformLocation(prog, "u_MVP");
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));

    int modelLoc = glGetUniformLocation(prog, "u_Model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    int viewLoc = glGetUniformLocation(prog, "u_View");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    int projLoc = glGetUniformLocation(prog, "u_Projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projMatrix));
}

void Renderer::setLazyWireframeMode(bool set){
    set ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

unsigned int Renderer::createVertexArrayObject() {
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    return VAO;
}

unsigned int Renderer::createVertexBufferObject(const float vertexData[], GLsizei bufferSize) {
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, bufferSize, vertexData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return VBO;
}

unsigned int Renderer::createElementBufferObject(const int index[], GLsizei bufferSize) {
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferSize, index, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    return EBO;
}

RenderData Renderer::createRenderData(const float vertexData[], GLsizei vertexDataSize, const int index[], GLsizei indexSize) {
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

    //cout << "Render data created! VAO: " << rData.VAO << ", VBO: " << rData.VBO << ", EBO: " << rData.EBO << ".\n";

    // Unbinding after finishing simply for the sake of better binding understanding.
    glBindVertexArray(0);
    return rData;
}

void Renderer::destroyRenderData(RenderData rData) {
    glDeleteBuffers(1, &rData.EBO);
    glDeleteBuffers(1, &rData.VBO);
    glDeleteVertexArrays(1, &rData.VAO);

    //printf("Render data destroyed.\n");
}

void Renderer::drawElements(RenderData rData, GLsizei indexSize) {
    setUpMVP();
    glBindVertexArray(rData.VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rData.EBO);

    glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::bindSolidColor() {
    GLint prog = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prog);

    glUniform1i(glGetUniformLocation(prog, "u_UseTexture"), 0);
}

void Renderer::bindTexture(unsigned int textureID) {
    GLint prog = 0;
    glActiveTexture(GL_TEXTURE0);
    glGetIntegerv(GL_CURRENT_PROGRAM, &prog);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glUniform1i(glGetUniformLocation(prog, "u_Texture"), 0);
    glUniform1i(glGetUniformLocation(prog, "u_UseTexture"), 1);
}

void Renderer::drawSolidColor(RenderData rData, GLsizei indexSize) {
    bindSolidColor();
    drawElements(rData, indexSize);
}

void Renderer::drawTexture(RenderData rData, GLsizei indexSize, unsigned int textureID) {
    bindTexture(textureID);
    drawElements(rData, indexSize);
}

void Renderer::getTextureSize(unsigned int textureID, int* width, int* height) {
    bindTexture(textureID);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, height);
    bindTexture(0);
}

void Renderer::setLightingData(const LightingData& data) {
    GLint prog = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prog);

    auto setVec3 = [&](const std::string& name, const glm::vec3& v) {
        int loc = glGetUniformLocation(prog, name.c_str());
        if (loc != -1) glUniform3f(loc, v.x, v.y, v.z);
    };

    auto setFloat = [&](const std::string& name, float v) {
        int loc = glGetUniformLocation(prog, name.c_str());
        if (loc != -1) glUniform1f(loc, v);
    };

    setVec3("u_viewPos", cameraPos);

    // Directional Light

    if (data.hasDirectional) {
        setVec3("u_dirLight.direction", static_cast<glm::vec3>(data.directionalLight->getDirection()));
        setVec3("u_dirLight.color", static_cast<glm::vec3>(data.directionalLight->getColor()));
    } else {
        setVec3("u_dirLight.color", {0.0f, 0.0f, 0.0f});
    }

    // PointLight

    int count = std::min(static_cast<int>(data.pointLights.size()), MAX_POINT_LIGHTS);

    for (int i = 0; i < count; i++) {
        const PointLight& l = *data.pointLights[i];
        std::string base = "u_pointLights[" + std::to_string(i) + "]";

        setVec3(base + ".position", static_cast<glm::vec3>(l.getPosition()));
        setVec3(base + ".color", static_cast<glm::vec3>(l.getColor()));
        setFloat(base + ".constant", l.getConstant());
        setFloat(base + ".linear", l.getLinear());
        setFloat(base + ".quadratic", l.getQuadratic());
    }

    for (int i = count; i < MAX_POINT_LIGHTS; i++) {
        std::string base = "u_pointLights[" + std::to_string(i) + "]";

        setVec3(base + ".position", {0,0,0});
        setVec3(base + ".color", {0,0,0});
        setFloat(base + ".constant", 1.0f);
        setFloat(base + ".linear", 0.0f);
        setFloat(base + ".quadratic", 0.0f);
    }

    // SpotLight

    int spotCount = std::min(static_cast<int>(data.spotLights.size()), MAX_SPOT_LIGHTS);

    for (int i = 0; i < spotCount; i++) {
        const SpotLight& l = *data.spotLights[i];
        std::string base = "u_spotLights[" + std::to_string(i) + "]";

        setVec3(base + ".position", static_cast<glm::vec3>(l.getPosition()));
        setVec3(base + ".direction", static_cast<glm::vec3>(l.getDirection()));
        setVec3(base + ".color", static_cast<glm::vec3>(l.getColor()));

        setFloat(base + ".cutOff", l.getCutOffCos());
        setFloat(base + ".outerCutOff", l.getOuterCutOffCos());

        setFloat(base + ".constant", l.getConstant());
        setFloat(base + ".linear", l.getLinear());
        setFloat(base + ".quadratic", l.getQuadratic());
    }

    for (int i = spotCount; i < MAX_SPOT_LIGHTS; i++) {
        std::string base = "u_spotLights[" + std::to_string(i) + "]";

        setVec3(base + ".position", {0,0,0});
        setVec3(base + ".direction", {0,0,0});
        setVec3(base + ".color", {0,0,0});

        setFloat(base + ".cutOff", 0.0f);
        setFloat(base + ".outerCutOff", 0.0f);

        setFloat(base + ".constant", 1.0f);
        setFloat(base + ".linear", 0.0f);
        setFloat(base + ".quadratic", 0.0f);
    }
}

void Renderer::setMaterial(const Material &material) {
    GLint prog = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prog);

    int materialAmbientLoc = glGetUniformLocation(prog, "u_material.ambient");
    int materialDiffuseLoc = glGetUniformLocation(prog, "u_material.diffuse");
    int materialSpecularLoc = glGetUniformLocation(prog, "u_material.specular");
    int materialShininessLoc = glGetUniformLocation(prog, "u_material.shininess");
    glUniform3f(materialAmbientLoc, material.ambient.x, material.ambient.y, material.ambient.z);
    glUniform3f(materialDiffuseLoc, material.diffuse.x, material.diffuse.y, material.diffuse.z);
    glUniform3f(materialSpecularLoc, material.specular.x, material.specular.y, material.specular.z);
    glUniform1f(materialShininessLoc, material.shininess);
}

void Renderer::setCameraPos(glm::vec3 newCameraPos) {
    cameraPos = newCameraPos;
}

void Renderer::setModelMatrix(glm::mat4 newModelMatrix) {
    modelMatrix = newModelMatrix;
}

void Renderer::setViewMatrix(glm::mat4 newViewMatrix) {
    viewMatrix = newViewMatrix;
}

void Renderer::setViewMatrix(glm::mat4 newViewMatrix, glm::vec3 newCameraPos) {
    setViewMatrix(newViewMatrix);
    setCameraPos(newCameraPos);
}

void Renderer::setOrthoProjectionMatrix(float width, float height) {
    projMatrix = glm::ortho(0.0f, width, height, 0.0f, -1.0f, 1.0f);
}

void Renderer::setPerspectiveProjectionMatrix(float width, float height, float fov, float farPlane) {
    projMatrix = glm::perspective(glm::radians(fov), width / height, 0.1f, farPlane);
}

void Renderer::setLightPos(glm::vec3 newLightPos) {
    lightPos = newLightPos;
}

void Renderer::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
