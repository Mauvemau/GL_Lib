#include "window.h"
#include "renderer.h"

#include <iostream>

using namespace std;

int main() {
    /* Initialize the library */
    if (!glfwInit()) {
        cout << "Failed to initialize glfw!\n";
        return -1;
    }
    else {
        cout << "GLFW initialized successfully!\n";
    }

    /* Create a windowed mode window and its OpenGL context */
    GLlib::Window window(640, 480, "Hello Triangle");

    /* Confirm that the window has been properly initialized */
    if (!window.getIsInitialized()) {
        cout << "Failed to create window!\n";
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    window.makeContextCurrent();

    /* Check if the PC has a working OpenGL driver */
    cout << glGetString(GL_VERSION) << "\n";

    cout << "Render loop has started!\n";
    /* Loop until the user closes the window */
    while (!window.getShouldClose())
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        window.swapBuffers();

        /* Poll for and process events */
        glfwPollEvents();
    }
    cout << "Render loop ended\n";

    /* Terminate the glfw context and window */
    glfwTerminate();

	return 0;
}