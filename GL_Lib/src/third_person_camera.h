#pragma once

#include "camera.h"

namespace gllib {
    class ThirdPersonCamera : public Camera {
    private:
        Vector3 target;

        float sensitivity;
        float distance;

        float pitch;
        float yaw;

        float maxRotationX = 89.0f;
        float minRotationX = -89.0f;

    public:
        ThirdPersonCamera(Vector3 initPosition, Vector3 initDirection, float sensitivity, float distance);
        ~ThirdPersonCamera() override;

        void setMinMaxClampX(float max, float min);
        void setSensitivity(float sensitivity);

        void updateMouseInput();
        void updateCamera(Vector3 target);
    };
}