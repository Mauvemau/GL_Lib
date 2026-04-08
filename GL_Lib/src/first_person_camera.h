#pragma once

#include "camera.h"

namespace gllib {

    class DLLExport FirstPersonCamera : public Camera{
    private:
        Vector3* target;
        float sensitivity;

        float pitch;
        float yaw;

        float maxRotationX = 89.0f;
        float minRotationX = -89.0f;
    public:
        FirstPersonCamera(Vector3 *target, Vector3 initDirection, float sensitivity);
        ~FirstPersonCamera() override;

        void setMinMaxClampX(float max, float min);
        void setSensitivity(float sensitivity);

        void updateMouseInput();
    };

}