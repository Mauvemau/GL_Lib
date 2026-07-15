#include "model_node.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace gllib;
using namespace std;

ModelNode::ModelNode(int id) : id(id), meshIndex(-1), parent(nullptr), localMatrix(1.0f), worldMatrix(1.0f), isDirty(true) {
    transform.position = { 0.0f, 0.0f, 0.0f };
    transform.rotationQuat = { 0.0f, 0.0f, 0.0f, 0.0f };
    transform.scale = { 0.01f, 0.01f, 0.01f };
    localAABB = { {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
    worldAABB = { {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
}

ModelNode::ModelNode(int id, int meshIndex, const BoundingBox& localBounds) :
id(id), meshIndex(meshIndex), localAABB(localBounds), parent(nullptr), localMatrix(1.0f), worldMatrix(1.0f), isDirty(true) {
    transform.position = { 0.0f, 0.0f, 0.0f };
    transform.rotationQuat = { 0.0f, 0.0f, 0.0f, 0.0f };
    transform.scale = { 0.01f, 0.01f, 0.01f };
    worldAABB = { {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
}

ModelNode::~ModelNode() {
    for (ModelNode* child : children) {
        delete child;
    }
    children.clear();
}

void ModelNode::addChild(ModelNode* child) {
    if (child != nullptr) {
        child->parent = this;
        child->isDirty = true;
        children.push_back(child);
    }
}

const std::vector<ModelNode*>& ModelNode::getChildren() const {
    return children;
}

ModelNode* ModelNode::getParent() const {
    return parent;
}

void ModelNode::setTransform(const Transform& newTransform) {
    transform = newTransform;
    isDirty = true;
}

void ModelNode::setLocalPosition(const Vector3& position) {
    transform.position = position;
    isDirty = true;
}

void ModelNode::setLocalRotation(const Vector3& rotationEuler) {
    transform.rotationQuat.x = rotationEuler.x;
    transform.rotationQuat.y = rotationEuler.y;
    transform.rotationQuat.z = rotationEuler.z;
    isDirty = true;
}

void ModelNode::setLocalScale(const Vector3& scale) {
    transform.scale = scale;
    isDirty = true;
}

int ModelNode::getId() const {
    return id;
}

const Transform& ModelNode::getTransform() const {
    return transform;
}

Vector3 ModelNode::getLocalPosition() const {
    return transform.position;
}

Quaternion ModelNode::getLocalRotation() const {
    return transform.rotationQuat;
}

Vector3 ModelNode::getLocalScale() const {
    return transform.scale;
}

const glm::mat4& ModelNode::getLocalMatrix() const {
    return localMatrix;
}

const glm::mat4& ModelNode::getWorldMatrix() const {
    return worldMatrix;
}

int ModelNode::getMeshIndex() const {
    return meshIndex;
}

const BoundingBox& ModelNode::getLocalBoundingBox() const {
    return localAABB;
}

const BoundingBox& ModelNode::getWorldBoundingBox() const {
    return worldAABB;
}

void ModelNode::updateTransformsAndBounds(const glm::mat4& parentWorldMatrix, bool parentWasDirty) {
    bool mustUpdate = isDirty || parentWasDirty;

    if (mustUpdate) {
        glm::mat4 trs = glm::mat4(1.0f);

        trs = glm::translate(trs, glm::vec3(transform.position.x, transform.position.y, transform.position.z));

        trs = glm::rotate(trs, glm::radians(transform.rotationQuat.x), glm::vec3(1.0f, 0.0f, 0.0f));
        trs = glm::rotate(trs, glm::radians(transform.rotationQuat.y), glm::vec3(0.0f, 1.0f, 0.0f));
        trs = glm::rotate(trs, glm::radians(transform.rotationQuat.z), glm::vec3(0.0f, 0.0f, 1.0f));

        trs = glm::scale(trs, glm::vec3(transform.scale.x, transform.scale.y, transform.scale.z));

        localMatrix = trs;
        worldMatrix = parentWorldMatrix * localMatrix;

        isDirty = false;
    }

    for (ModelNode* child : children) {
        child->updateTransformsAndBounds(worldMatrix, mustUpdate);
    }

    if (mustUpdate) {
        calculateBoundingBox();
    }
}

bool ModelNode::hasMesh() const {
    return meshIndex != -1;
}

void ModelNode::calculateBoundingBox() {
    if (hasMesh()) { // Has geometry
        glm::vec3 corners[8];
        corners[0] = glm::vec3(localAABB.min.x, localAABB.min.y, localAABB.min.z);
        corners[1] = glm::vec3(localAABB.min.x, localAABB.min.y, localAABB.max.z);
        corners[2] = glm::vec3(localAABB.min.x, localAABB.max.y, localAABB.min.z);
        corners[3] = glm::vec3(localAABB.min.x, localAABB.max.y, localAABB.max.z);
        corners[4] = glm::vec3(localAABB.max.x, localAABB.min.y, localAABB.min.z);
        corners[5] = glm::vec3(localAABB.max.x, localAABB.min.y, localAABB.max.z);
        corners[6] = glm::vec3(localAABB.max.x, localAABB.max.y, localAABB.min.z);
        corners[7] = glm::vec3(localAABB.max.x, localAABB.max.y, localAABB.max.z);

        glm::vec4 firstTransformed = worldMatrix * glm::vec4(corners[0], 1.0f);
        worldAABB.min = { firstTransformed.x, firstTransformed.y, firstTransformed.z };
        worldAABB.max = { firstTransformed.x, firstTransformed.y, firstTransformed.z };

        for (int i = 1; i < 8; ++i) {
            glm::vec4 transformed = worldMatrix * glm::vec4(corners[i], 1.0f);

            if (transformed.x < worldAABB.min.x) worldAABB.min.x = transformed.x;
            if (transformed.y < worldAABB.min.y) worldAABB.min.y = transformed.y;
            if (transformed.z < worldAABB.min.z) worldAABB.min.z = transformed.z;

            if (transformed.x > worldAABB.max.x) worldAABB.max.x = transformed.x;
            if (transformed.y > worldAABB.max.y) worldAABB.max.y = transformed.y;
            if (transformed.z > worldAABB.max.z) worldAABB.max.z = transformed.z;
        }
    }
    else if (!children.empty()) { // Empty branch node
        worldAABB = children[0]->getWorldBoundingBox();
    }
    else { // Empty leaf
        Vector3 pos = { worldMatrix[3].x, worldMatrix[3].y, worldMatrix[3].z };
        worldAABB.min = pos;
        worldAABB.max = pos;
    }

    // Expanding parent with children
    for (ModelNode* child : children) {
        const BoundingBox& childBox = child->getWorldBoundingBox();

        if (childBox.min.x < worldAABB.min.x) worldAABB.min.x = childBox.min.x;
        if (childBox.min.y < worldAABB.min.y) worldAABB.min.y = childBox.min.y;
        if (childBox.min.z < worldAABB.min.z) worldAABB.min.z = childBox.min.z;

        if (childBox.max.x > worldAABB.max.x) worldAABB.max.x = childBox.max.x;
        if (childBox.max.y > worldAABB.max.y) worldAABB.max.y = childBox.max.y;
        if (childBox.max.z > worldAABB.max.z) worldAABB.max.z = childBox.max.z;
    }
}

ModelNode* ModelNode::clone() const {
    ModelNode* cloned = nullptr;
    if (this->hasMesh()) {
        cloned = new ModelNode(this->id, this->meshIndex, this->localAABB);
    } else {
        cloned = new ModelNode(this->id);
    }

    cloned->transform = this->transform;
    cloned->localMatrix = this->localMatrix;
    cloned->worldMatrix = this->worldMatrix;
    cloned->isDirty = true;

    for (ModelNode* child : this->children) {
        cloned->addChild(child->clone());
    }

    return cloned;
}