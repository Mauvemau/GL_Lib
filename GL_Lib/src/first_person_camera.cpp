#include "first_person_camera.h"
#include "input.h"
#include "myMaths.h"

#include <iostream>

using namespace std;

namespace gllib {

    FirstPersonCamera::FirstPersonCamera(Vector3 initPosition, Vector3 initDirection, float sensitivity) {
        this->position = initPosition;
        initDirection = initDirection.normalized();
        yaw = glm::degrees(atan2(initDirection.z, initDirection.x));
        pitch = glm::degrees(asin(initDirection.y));
        rotate(pitch, yaw);
        update();

        this->sensitivity = sensitivity;
        cout << "Created FP Camera.\n";
    }

    FirstPersonCamera::~FirstPersonCamera()  {
        cout << "Destroyed FP Camera.\n";
    }

    void FirstPersonCamera::setMinMaxClampX(float max, float min) {
        maxRotationX = max;
        minRotationX = min;
    }

    void FirstPersonCamera::setSensitivity(float sensitivity) {
        this->sensitivity = sensitivity;
    }

    void FirstPersonCamera::followTargetPosition(Vector3 targetPosition) {
        position = targetPosition;
        update();
    }

    void FirstPersonCamera::updateMouseInput() {
        float mouseX = Input::getMouseDeltaX();
        float mouseY = Input::getMouseDeltaY();

        yaw += mouseX * sensitivity;
        pitch += mouseY * sensitivity;

        pitch = Maths::clamp(pitch, minRotationX, maxRotationX);

        rotate(pitch, yaw);

        update();
    }

}