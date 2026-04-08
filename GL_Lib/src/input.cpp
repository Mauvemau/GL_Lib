#include "input.h"

#include <unordered_map>

GLFWwindow* Input::window = nullptr;
float Input::deltaX = 0;
float Input::deltaY = 0;

float Input::lastX = 0;
float Input::lastY = 0;
bool Input::firstMouse = true;

Input::Input(GLFWwindow* window){
    Input::window = window;
    glfwSetCursorPosCallback(window, mouseCallback);
}

void Input::mouseCallback(GLFWwindow* window, double xPos, double yPos) {
    if (firstMouse) {
        lastX = static_cast<float>(xPos);
        lastY = static_cast<float>(yPos);
        firstMouse = false;
    }

    deltaX = static_cast<float>(xPos )- lastX;
    deltaY = lastY - static_cast<float>(yPos);

    lastX = static_cast<float>(xPos);
    lastY = static_cast<float>(yPos);
}

bool Input::getKeyPressed(int key)
{
    return (glfwGetKey(window, key) == GLFW_PRESS);
}

bool Input::getMouseButtonDown(int button)
{
    return (glfwGetMouseButton(window, button) == GLFW_PRESS);
}

bool Input::getMouseButtonUp(int button)
{
    return (glfwGetMouseButton(window, button) == GLFW_RELEASE);
}

float Input::getMouseX() 
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return xpos;
}

float Input::getMouseY()
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return ypos;
}

float Input::getMouseDeltaX() {
    float dx = deltaX;
    deltaX = 0.0f;
    return dx;
}

float Input::getMouseDeltaY() {
    float dy = deltaY;
    deltaY = 0.0f;
    return dy;
}

void Input::setCursorLocked(bool lock) {
    glfwSetInputMode(window, GLFW_CURSOR, lock ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

bool Input::isCursorLocked() {
    return glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
}

bool Input::isAnyKeyPressed()
{
    for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; ++key)
    {
        if (glfwGetKey(window, key) == GLFW_PRESS)
        {
            return true;
        }
    }
    return false;
}

bool Input::getKeyReleased(int key)
{
    static std::unordered_map<int, bool> keyStates;

    if (glfwGetKey(window, key) == GLFW_RELEASE)
    {
        if (keyStates[key])
        {
            keyStates[key] = false;
            return true;
        }
    }
    else
    {
        keyStates[key] = true;
    }

    return false;
}
