#pragma once

#include "camera.h"

namespace gllib {

    class DLLExport FirstPersonCamera : public Camera{
    private:
        float sensitivity;

        float pitch;
        float yaw;

        float maxRotationX = 89.0f;
        float minRotationX = -89.0f;

    public:
        FirstPersonCamera(Vector3 initPosition, Vector3 initDirection, float sensitivity);
        ~FirstPersonCamera() override;

        void setMinMaxClampX(float max, float min);
        void setSensitivity(float sensitivity);

        void followTargetPosition(Vector3 targetPosition);
        void updateMouseInput();
    };

}