#include "third_person_camera.h"
#include "../input.h"
#include "../myMaths.h"

#include <iostream>

using namespace std;

namespace gllib {

    ThirdPersonCamera::ThirdPersonCamera(Vector3 initPosition, Vector3 initDirection, float sensitivity, float distance) {
        this->position = initPosition;
        initDirection = initDirection.normalized();
        yaw = glm::degrees(atan2(initDirection.z, initDirection.x));
        pitch = glm::degrees(asin(initDirection.y));

        updateCamera(initPosition);

        this->sensitivity = sensitivity;
        this->distance = distance;
        cout << "Created TP Camera.\n";
    }

    ThirdPersonCamera::~ThirdPersonCamera() {
        cout << "Destroyed TP Camera.\n";
    }

    void ThirdPersonCamera::setMinMaxClampX(float max, float min) {
        maxRotationX = max;
        minRotationX = min;
    }

    void ThirdPersonCamera::setSensitivity(float sensitivity) {
        this->sensitivity = sensitivity;
    }

    void ThirdPersonCamera::updateMouseInput() {
        float mouseX = Input::getMouseDeltaX();
        float mouseY = Input::getMouseDeltaY();

        yaw   += mouseX * sensitivity;
        pitch += mouseY * sensitivity;

        pitch = Maths::clamp(pitch, minRotationX, maxRotationX);
    }

    void ThirdPersonCamera::updateCamera(Vector3 target) {
        this->target = target;

        Vector3 offset;
        offset.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        offset.y = sin(glm::radians(pitch));
        offset.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        offset = offset.normalized() * distance;

        position = target - offset;

        direction = (target - position).normalized();

        update();
    }

}