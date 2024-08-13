#include "core.h"
#include "deps.h"

#include <iostream>

using namespace gllib;
using namespace std;

LibCore::LibCore() {
    if (!glfwInit()) {
        cout << "Failed to initialize glfw!\n";
    }
    else {
        cout << "GLFW initialized successfully!\n";
    }
}

LibCore::~LibCore() {
    cout << "Terminated glfw.\n";
    glfwTerminate();
}

bool LibCore::initGlad() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD\n";
        glfwTerminate();
        return false;
    }
    else {
        cout << "Initialized GLAD successfully!\n";
    }
    return true;
}


void LibCore::pollEvents() {
    glfwPollEvents();
}