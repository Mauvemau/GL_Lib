#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "transform.h"

namespace gllib {

    struct DLLExport BoundingBox {
        Vector3 min;
        Vector3 max;
    };

    class DLLExport ModelNode {
    private:
        int id;
        Transform transform;
        glm::mat4 localMatrix;
        glm::mat4 worldMatrix;

        bool isDirty;

        ModelNode* parent;
        std::vector<ModelNode*> children;

        int meshIndex;
        BoundingBox localAABB;
        BoundingBox worldAABB;

        bool isBSPPlane = false;

        void calculateBoundingBox();

    public:
        ModelNode(int id);
        ModelNode(int id, int meshIndex, const BoundingBox& localBounds);
        ~ModelNode();

        void addChild(ModelNode* child);
        const std::vector<ModelNode*>& getChildren() const;
        ModelNode* getParent() const;

        void setTransform(const Transform& newTransform);
        void setLocalPosition(const Vector3& position);
        void setLocalRotation(const Vector3& rotationEuler);
        void setLocalScale(const Vector3& scale);
        void setIsBSPPlane(bool bsp);

        const Transform& getTransform() const;
        Vector3 getLocalPosition() const;
        Quaternion getLocalRotation() const;
        Vector3 getLocalScale() const;

        const glm::mat4& getLocalMatrix() const;
        const glm::mat4& getWorldMatrix() const;

        int getId() const;
        int getMeshIndex() const;
        const BoundingBox& getLocalBoundingBox() const;
        const BoundingBox& getWorldBoundingBox() const;
        bool getIsBSPPlane() const;

        bool hasMesh() const;

        void updateTransformsAndBounds(const glm::mat4& parentWorldMatrix, bool parentWasDirty);
        ModelNode* clone() const;
    };

}