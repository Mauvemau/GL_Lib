#include "shape_group.h"

#include <iostream>

using namespace gllib;
using namespace std;

ShapeGroup::ShapeGroup(Vector3 translation, Vector3 rotation, Vector3 scale) :
Entity(translation, rotation, scale) {
    cout << "Created shape group.\n";
}

ShapeGroup::ShapeGroup(Transform transform) :
Entity(transform) {
    cout << "Created shape group.\n";
}

ShapeGroup::~ShapeGroup() {
    cout << "Destroyed shape group.\n";

    for (RenderData& renderData : renderDatas) {
        Renderer::destroyRenderData(renderData);
    }
}

void ShapeGroup::addRenderData(const float vertexData[], int vertexDataSize, const int index[], int indexSize) {
    RenderData renderData = Renderer::createRenderData(vertexData, vertexDataSize, index, indexSize);

    renderDatas.push_back(renderData);
    indexSizes.push_back(indexSize);
    materials.push_back(Material());
}

void ShapeGroup::pivotVertex(float* vertexData, int vertexCount, int vertexStride, Vector3 pivot) {
    for (int i = 0; i < vertexCount; i++) {
        vertexData[i * vertexStride] -= pivot.x;
        vertexData[i * vertexStride + 1] -= pivot.y;
        vertexData[i * vertexStride + 2] -= pivot.z;
    }
}

Vector3 ShapeGroup::calculateBoundsCenter(const vector<vector<float>>& vertexGroups, int vertexStride) {
    float minX = FLT_MAX;
    float minY = FLT_MAX;
    float minZ = FLT_MAX;
    float maxX = -FLT_MAX;
    float maxY = -FLT_MAX;
    float maxZ = -FLT_MAX;

    for (const vector<float>& vertices : vertexGroups) {
        int vertexCount = vertices.size() / vertexStride;

        for (int i = 0; i < vertexCount; i++) {
            float x = vertices[i * vertexStride];
            float y = vertices[i * vertexStride + 1];
            float z = vertices[i * vertexStride + 2];

            minX = min(minX, x);
            minY = min(minY, y);
            minZ = min(minZ, z);

            maxX = max(maxX, x);
            maxY = max(maxY, y);
            maxZ = max(maxZ, z);
        }
    }

    return Vector3((minX + maxX) * 0.5f, (minY + maxY) * 0.5f, (minZ + maxZ) * 0.5f);
}

void ShapeGroup::internalDraw() {
    glm::mat4 trs = glm::mat4(1.0f);

    trs = glm::translate(glm::mat4(1.0f), glm::vec3(transform.position.x,transform.position.y, transform.position.z));
    trs = glm::rotate(trs, glm::radians(transform.rotationQuat.x), glm::vec3(1.0, 0.0f, 0.0f));
    trs = glm::rotate(trs, glm::radians(transform.rotationQuat.y), glm::vec3(0.0f, 1.0f, 0.0f));
    trs = glm::rotate(trs, glm::radians(transform.rotationQuat.z), glm::vec3(0.0f, 0.0f, 1.0f));
    trs = glm::scale(trs, glm::vec3(transform.scale.x, transform.scale.y, transform.scale.z));

    Renderer::setModelMatrix(trs);

    for (int i = 0; i < renderDatas.size(); i++) {
        const Material& material = materials[i];
        Renderer::setMaterial(material);

        if (material.texture > 0) {
            Renderer::drawTexture(renderDatas[i], indexSizes[i], material.texture);
        }
        else {
            Renderer::drawSolidColor(renderDatas[i], indexSizes[i]);
        }
    }
}