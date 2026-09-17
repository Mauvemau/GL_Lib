#include "model.h"
#include <iostream>
#include "../renderer.h"
#include "../lighting/material_group.h"

using namespace gllib;
using namespace std;

Model::Model(ModelData& modelData, Transform transform, Color color) :
    ShapeGroup(transform),
    meshGroup(modelData.getMeshGroup()->clone()),
    lastFrameDrawnCount(-1),
    totalMeshesCount(0) {

    this->color = color;

    if (modelData.getRootNode() != nullptr) {
        this->rootNode = modelData.getRootNode()->clone();
        collectBSPPlanes(rootNode);
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

    if (meshGroup != nullptr) {
        delete meshGroup;
        meshGroup = nullptr;
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

void Model::printBSPPlanes() {
    cout << "BSP Nodes: " << bspNodes.size() << endl;
    cout << "BSP Planes: " << bspPlanes.size() << endl;
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

void Model::drawBSPPlanesDebug() {
    if (!Renderer::isDebug()) return;

    for (const BSPPlane& plane : bspPlanes) {
        Vector3 planePoint = Vector3(plane.point.x, plane.point.y, plane.point.z);
        Vector3 planeNormal = Vector3(plane.normal.x, plane.normal.y, plane.normal.z);

        BoundingBox originBox;
        originBox.min = planePoint - Vector3(0.15f, 0.15f, 0.15f);
        originBox.max = planePoint + Vector3(0.15f, 0.15f, 0.15f);
        Renderer::drawBoundingBox(originBox, glm::mat4(1.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)); // Magenta

        Vector3 normalEnd = planePoint + (planeNormal * 2.0f);
        Renderer::drawLine(planePoint, normalEnd, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)); // Amarillo
    }
}

int Model::countTotalMeshesRecursive(ModelNode* node) const {
    if (node == nullptr) return 0;

    int count = node->hasMesh() ? 1 : 0;
    for (ModelNode* child : node->getChildren()) {
        count += countTotalMeshesRecursive(child);
    }
    return count;
}

void Model::renderNodeRecursive(ModelNode* node, const Frustum& frustum, int& drawnCounter) {
    if (node == nullptr) return;

    if (!frustum.isBoxInFrustum(node->getWorldBoundingBox())) {
        return;
    }

    if (node->hasMesh()) {
        drawSubMesh(node->getMeshIndex(), node->getWorldMatrix());
        drawnCounter++;
    }

    if (Renderer::isDebug()) {
        glm::vec4 boxColor = node->hasMesh() ? glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)
                                             : glm::vec4(0.0f, 0.7f, 1.0f, 1.0f);
        Renderer::drawBoundingBox(node->getWorldBoundingBox(), glm::mat4(1.0f), boxColor);
    }

    for (ModelNode* child : node->getChildren()) {
        renderNodeRecursive(child, frustum, drawnCounter);
    }
}

void Model::collectBSPPlanes(ModelNode* node){
    if (!node) {
        return;
    }

    if (node->getIsBSPPlane()) {
        bspNodes.push_back(node);
    }

    const std::vector<ModelNode*>& children = node->getChildren();

    for (ModelNode* child : children) {
        collectBSPPlanes(child);
    }
}

void Model::updateBSPPlanes() {
    bspPlanes.clear();

    vector<Mesh>& meshes = meshGroup->getMeshes();

    for (ModelNode* bspNode : bspNodes) {
        if (bspNode == nullptr) {
            continue;
        }

        const vector<ModelNode*>& children = bspNode->getChildren();

        if (children.empty()) {
            continue;
        }

        ModelNode* meshNode = children[0];

        if (meshNode == nullptr || !meshNode->hasMesh()) {
            continue;
        }

        int meshIndex = meshNode->getMeshIndex();

        if (meshIndex < 0 || meshIndex >= static_cast<int>(meshes.size())) {
            continue;
        }

        Mesh& mesh = meshes[meshIndex];

        vector<Vertex>& vertices = mesh.getVertices();
        vector<int>& indices = mesh.getIndices();

        if (vertices.empty() || indices.size() < 3) {
            continue;
        }

        int i0 = indices[0];
        int i1 = indices[1];
        int i2 = indices[2];

        if (i0 < 0 || i0 >= static_cast<int>(vertices.size()) ||
            i1 < 0 || i1 >= static_cast<int>(vertices.size()) ||
            i2 < 0 || i2 >= static_cast<int>(vertices.size())) {
            continue;
        }

        const glm::mat4& worldMatrix = meshNode->getWorldMatrix();

        glm::vec3 v0 = glm::vec3(
            worldMatrix * glm::vec4(
                vertices[i0].position.x,
                vertices[i0].position.y,
                vertices[i0].position.z,
                1.0f));

        glm::vec3 v1 = glm::vec3(
            worldMatrix * glm::vec4(
                vertices[i1].position.x,
                vertices[i1].position.y,
                vertices[i1].position.z,
                1.0f));

        glm::vec3 v2 = glm::vec3(
            worldMatrix * glm::vec4(
                vertices[i2].position.x,
                vertices[i2].position.y,
                vertices[i2].position.z,
                1.0f));

        glm::vec3 edge1 = v1 - v0;
        glm::vec3 edge2 = v2 - v0;

        glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

        glm::vec3 center(0.0f);

        for (const Vertex& vertex : vertices) {
            center += glm::vec3(
                vertex.position.x,
                vertex.position.y,
                vertex.position.z);
        }

        center /= static_cast<float>(vertices.size());

        BSPPlane plane;
        plane.point = glm::vec3(worldMatrix * glm::vec4(center, 1.0f));
        plane.normal = normal;

        bspPlanes.push_back(plane);
    }
}

void Model::draw() {
    if (rootNode == nullptr) return;

    if (totalMeshesCount == 0) {
        totalMeshesCount = countTotalMeshesRecursive(rootNode);
    }

    glm::mat4 baseWorldMatrix = glm::mat4(1.0f);
    baseWorldMatrix = glm::translate(baseWorldMatrix, glm::vec3(transform.position.x, transform.position.y, transform.position.z));
    baseWorldMatrix = glm::rotate(baseWorldMatrix, glm::radians(transform.rotationQuat.x), glm::vec3(1.0f, 0.0f, 0.0f));
    baseWorldMatrix = glm::rotate(baseWorldMatrix, glm::radians(transform.rotationQuat.y), glm::vec3(0.0f, 1.0f, 0.0f));
    baseWorldMatrix = glm::rotate(baseWorldMatrix, glm::radians(transform.rotationQuat.z), glm::vec3(0.0f, 0.0f, 1.0f));
    baseWorldMatrix = glm::scale(baseWorldMatrix, glm::vec3(transform.scale.x, transform.scale.y, transform.scale.z));

    rootNode->updateTransformsAndBounds(baseWorldMatrix, true);
    updateBSPPlanes();

    Frustum frustum;
    frustum.update(Renderer::getViewMatrix(), Renderer::getProjMatrix());

    int drawnCounter = 0;
    renderNodeRecursive(rootNode, frustum, drawnCounter);

    if (Renderer::isDebug()) {
        drawBSPPlanesDebug();
    }

    if (drawnCounter != lastFrameDrawnCount && Renderer::isDebug()) {
        cout << "[Culling] Current: " << drawnCounter
             << " / Total: " << totalMeshesCount << "\n";
        lastFrameDrawnCount = drawnCounter;
    }
}