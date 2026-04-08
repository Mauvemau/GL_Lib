#pragma once

#include "window.h"

enum Keys
{
    Key_A = 65,
    Key_B,
    Key_C,
    Key_D,
    Key_E,
    Key_F,
    Key_G,
    Key_H,
    Key_I,
    Key_J,
    Key_K,
    Key_L,
    Key_M,
    Key_N,
    Key_O,
    Key_P,
    Key_Q,
    Key_R,
    Key_S,
    Key_T,
    Key_U,
    Key_V,
    Key_W,
    Key_X,
    Key_Y,
    Key_Z,

    // Common keys
    Key_Space      = GLFW_KEY_SPACE,
    Key_Enter      = GLFW_KEY_ENTER,
    Key_Escape     = GLFW_KEY_ESCAPE,
    Key_Tab        = GLFW_KEY_TAB,
    Key_Backspace  = GLFW_KEY_BACKSPACE,

    // Modifiers
    Key_LeftShift  = GLFW_KEY_LEFT_SHIFT,
    Key_RightShift = GLFW_KEY_RIGHT_SHIFT,
    Key_LeftCtrl   = GLFW_KEY_LEFT_CONTROL,
    Key_RightCtrl  = GLFW_KEY_RIGHT_CONTROL,
    Key_LeftAlt    = GLFW_KEY_LEFT_ALT,
    Key_RightAlt   = GLFW_KEY_RIGHT_ALT,

    // Arrows
    Key_Up    = GLFW_KEY_UP,
    Key_Down  = GLFW_KEY_DOWN,
    Key_Left  = GLFW_KEY_LEFT,
    Key_Right = GLFW_KEY_RIGHT
};

class DLLExport Input {
private:
    static GLFWwindow* window;
    bool keys[GLFW_KEY_LAST + 1] = { false };
    bool mouseButtons[GLFW_MOUSE_BUTTON_LAST + 1] = { false };

    static float deltaX;
    static float deltaY;

    static float lastX;
    static float lastY;
    static bool firstMouse;
public:
    // Constructor
    Input(GLFWwindow* window);

    static void mouseCallback(GLFWwindow* window, double xPos, double yPos);

    // Key input functions
    static bool getKeyPressed(int key);
    static bool getKeyReleased(int key);

    // Mouse input functions
    static bool getMouseButtonDown(int button);
    static bool getMouseButtonUp(int button);
    
    static float getMouseX();
    static float getMouseY();
    static float getMouseDeltaX();
    static float getMouseDeltaY();
    static void setCursorLocked(bool lock);
    static bool isCursorLocked();
    static bool isAnyKeyPressed();
};
