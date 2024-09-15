﻿#pragma once
#include "transform.h"
#include "deps.h"

namespace gllib {
    
    class DLLExport Entity {
        
    protected:
        Transform transform;
        Color color;

    public:
        Entity(const Transform& transform);
        virtual ~Entity();

        void move(const Vector3& direction) const;
        void rotate(const Vector3& eulerRotation) const;
        void updateTransform();

        Vector3 upward() const;
        Vector3 forward() const;
        Vector3 right() const;
        
        Transform getTransform() const;
        Vector3 getPosition() const;
        Vector3 getScale() const;
        Vector3 getRotationEuler() const;
        Quaternion getRotationQuat() const;
        Color getColor() const;

        void setTransform(const Transform& transform);
        void setPosition(const Vector3& position);
        void setScale(const Vector3& scale);
        void setRotationQuat(const Quaternion& rotation);
        void setRotationEuler(const Vector3& rotation);
        void setColor(const Color& color);
        bool isColliding(const Transform& _transform) const;
        bool isColliding(float x, float y, float width, float height) const;

    };
}
