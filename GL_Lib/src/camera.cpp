#include "camera.h"
#include "renderer.h"

#include <iostream>

using namespace std;

namespace gllib {

    Camera::Camera() {
        position = Vector3(0.0f, 0.0f, -4.0f);
        lookAt(Vector3(0.0f, 0.0f, 0.0f));
    }

    Camera::Camera(Vector3 position, Vector3 direction) {
        this->position = position;
        lookAt(direction);
        cout << "Created Camera.\n";
    }

    Camera::~Camera() {
        cout << "Destroyed Camera.\n";
    }

    void Camera::update() {
        glm::mat4 view = glm::lookAt(static_cast<glm::vec3>(position),
            static_cast<glm::vec3>(position + forward()),
            static_cast<glm::vec3>(up()));
        Renderer::setViewMatrix(view, static_cast<glm::vec3>(position));
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

    void Camera::rotate(float x, float y) {
        this->direction.x = cos(glm::radians(y)) * cos(glm::radians(x));
        this->direction.y = sin(glm::radians(x));
        this->direction.z = sin(glm::radians(y)) * cos(glm::radians(x));
    }

    void Camera::lookAt(Vector3 target) {
        direction = (target - position).normalized();
    }

}