#include "camera.h"

namespace gllib {

    Camera::Camera() {

    }

    Camera::~Camera() {

    }

    Vector3 Camera::forwardWorld() {
        return Vector3(0.0f, 0.0f,  -1.0f);
    }

    Vector3 Camera::forward() {
        return direction.normalized();
    }

    Vector3 Camera::rightWorld() {
        return Vector3(1, 0, 0);
    }

    Vector3 Camera::right() {
        return Maths::cross(forward(), upWorld()).normalized();
    }

    Vector3 Camera::upWorld() {
        return Vector3(0.0f, 1.0f,  0.0f);
    }

    Vector3 Camera::up() {
        return Maths::cross(right(), forward()).normalized();
    }

    void Camera::move(Vector3 position) {
        this->position += position;
    }

    void Camera::rotate(Vector3 direction) {
        this->direction.x = cos(glm::radians(direction.y)) * cos(glm::radians(direction.x));
        this->direction.y = sin(glm::radians(direction.x));
        this->direction.z = sin(glm::radians(direction.y)) * cos(glm::radians(direction.x));
    }

    void Camera::update() {
        glm::mat4 view = glm::lookAt(static_cast<glm::vec3>(position),
            static_cast<glm::vec3>(position + forward()),
            static_cast<glm::vec3>(up()));
        Renderer::setViewMatrix(view);
    }

}