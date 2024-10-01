#include "Input.h"

GLFWwindow* Input::window = nullptr;

Input::Input(GLFWwindow* window)
{
    Input::window = window;
}

bool Input::getKeyDown(int key)
{
    return (glfwGetKey(window, key) == GLFW_PRESS);
}

bool Input::getKeyUp(int key)
{
    return (glfwGetKey(window, key) == GLFW_RELEASE);
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

bool Input::isAnyKeyDown()
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
