#include "model.h"
#include <iostream>
#include "../renderer.h"

using namespace gllib;
using namespace std;

Model::Model(ModelData& modelData, Transform transform, Color color) :
    ShapeGroup(transform), meshGroup(modelData.getMeshGroup()) {

    this->color = color;

    if (modelData.getRootNode() != nullptr) {
        this->rootNode = modelData.getRootNode()->clone();
    } else {
        this->rootNode = nullptr;
    }

    updateRenderData(this->color, false);

    vector<Material>& loadedMaterials = modelData.getMaterialGroup()->getMaterials();
    vector<Mesh>& meshes = meshGroup->getMeshes();

    for (size_t i = 0; i < meshes.size(); i++) {
        unsigned int matIdx = meshes[i].getMaterialIndex();

        if (matIdx < loadedMaterials.size()) {
            materials[i] = loadedMaterials[matIdx];
        }
    }

    cout << "Created model unified with ModelData and independent Node hierarchy.\n";
}

Model::~Model() {
    if (rootNode != nullptr) {
        delete rootNode;
        rootNode = nullptr;
    }
    cout << "Destroyed model.\n";
}

void Model::updateRenderData(Color color, bool centerPivot) {
    vector<Mesh>& meshes = meshGroup->getMeshes();
    vector<vector<float>> vertexGroups;

    for (Mesh& mesh : meshes) {
        vector<float> vertexData;
        vector<Vertex>& vertices = mesh.getVertices();

        for (Vertex& vertex : vertices) {
            // Position
            vertexData.push_back(vertex.position.x);
            vertexData.push_back(vertex.position.y);
            vertexData.push_back(vertex.position.z);
            // Normal
            vertexData.push_back(vertex.normal.x);
            vertexData.push_back(vertex.normal.y);
            vertexData.push_back(vertex.normal.z);
            // Color
            vertexData.push_back(color.r);
            vertexData.push_back(color.g);
            vertexData.push_back(color.b);
            vertexData.push_back(color.a);
            // UV
            vertexData.push_back(vertex.uv.x);
            vertexData.push_back(vertex.uv.y);
        }

        vertexGroups.push_back(vertexData);
    }

    Vector3 pivot(0,0,0);
    if (centerPivot) {
        pivot = calculateBoundsCenter(vertexGroups, 12);
    }

    for (size_t i = 0; i < meshes.size(); i++) {
        Mesh& mesh = meshes[i];

        vector<float>& vertexData = vertexGroups[i];
        vector<int>& indices = mesh.getIndices();

        if (centerPivot) {
            pivotVertex(vertexData.data(), static_cast<int>(vertexData.size() / 12), 12, pivot);
        }

        addRenderData(vertexData.data(), static_cast<int>(vertexData.size()), indices.data(), static_cast<int>(indices.size()));
    }
}

Color Model::getColor() {
    return color;
}

void Model::setMaterial(unsigned int index, const Material& material) {
    if (index >= materials.size()) {
        return;
    }
    materials[index] = material;
}

MeshGroup& Model::getMeshGroup() {
    return *meshGroup;
}

Mesh& Model::getMesh() {
    return meshGroup->getMeshes()[0];
}

ModelNode* Model::findNode(int id) {
    return findNodeRecursive(rootNode, id);
}

ModelNode* Model::findNodeRecursive(ModelNode* node, int id) {
    if (node == nullptr) return nullptr;

    if (node->getId() == id) {
        return node;
    }

    for (ModelNode* child : node->getChildren()) {
        ModelNode* found = findNodeRecursive(child, id);
        if (found != nullptr) {
            return found;
        }
    }

    return nullptr;
}

void Model::renderNodeRecursive(ModelNode* node) {
    if (node == nullptr) return;

    if (node->hasMesh()) {
        drawSubMesh(node->getMeshIndex(), node->getWorldMatrix());
    }

    for (ModelNode* child : node->getChildren()) {
        renderNodeRecursive(child);
    }
}

void Model::draw() {
    if (rootNode == nullptr) return;

    glm::mat4 baseWorldMatrix = glm::mat4(1.0f);
    baseWorldMatrix = glm::translate(baseWorldMatrix, glm::vec3(transform.position.x, transform.position.y, transform.position.z));
    baseWorldMatrix = glm::rotate(baseWorldMatrix, glm::radians(transform.rotationQuat.x), glm::vec3(1.0f, 0.0f, 0.0f));
    baseWorldMatrix = glm::rotate(baseWorldMatrix, glm::radians(transform.rotationQuat.y), glm::vec3(0.0f, 1.0f, 0.0f));
    baseWorldMatrix = glm::rotate(baseWorldMatrix, glm::radians(transform.rotationQuat.z), glm::vec3(0.0f, 0.0f, 1.0f));
    baseWorldMatrix = glm::scale(baseWorldMatrix, glm::vec3(transform.scale.x, transform.scale.y, transform.scale.z));

    rootNode->updateTransformsAndBounds(baseWorldMatrix, true);

    renderNodeRecursive(rootNode);
}