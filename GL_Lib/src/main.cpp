#include "window.h"
#include "renderer.h"
#include "shader.h"

#include <iostream>

using namespace std;

const float triangleVertexData[] = {
    -0.5f, -0.5f, 0.0f, /* xyz */ 1.0f, 0.0f, 0.0f, 0.0f, /* rgba */ 0.0f, 0.0f, /* uv */
     0.0f,  0.5f, 0.0f, /* xyz */ 0.0f, 1.0f, 0.0f, 1.0f, /* rgba */ 1.0f, 0.0f, /* uv */
     0.5f, -0.5f, 0.0f, /* xyz */ 0.0f, 0.0f, 1.0f, 0.5f, /* rgba */ 0.0f, 1.0f, /* uv */
};
const int triangleIndex[] = {
    0, 1, 2
};

int main() {
    // Initialize the GLFW library
    if (!glfwInit()) {
        cout << "Failed to initialize glfw!\n";
        return -1;
    }
    else {
        cout << "GLFW initialized successfully!\n";
    }

    // Create a window
    gllib::Window window(640, 480, "Hello Triangle");

    // Confirm that the window has been properly initialized
    if (!window.getIsInitialized()) {
        cout << "Failed to create window!\n";
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    window.makeContextCurrent();

    // Initialize the GLAD library
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD\n";
        glfwTerminate();
        return -1;
    }
    else {
        cout << "Initialized GLAD successfully!\n";
    }

    // Check if the PC has a working OpenGL driver
    cout << glGetString(GL_VERSION) << "\n";

    // Load vertex and fragment shaders from files
    const char* vertexSource = gllib::Shader::loadShader("solidColorV.glsl");
    const char* fragmentSource = gllib::Shader::loadShader("solidColorF.glsl");
    // Create shader program
    unsigned int spSolidColor = gllib::Shader::createShader(vertexSource, fragmentSource);
    // Set current shader program
    gllib::Shader::useShaderProgram(spSolidColor);

    // Get the size of the buffers
    GLsizei triangleVertexDataSize = sizeof(triangleVertexData) / sizeof(triangleVertexData[0]);
    GLsizei triangleIndexSize = sizeof(triangleIndex) / sizeof(triangleIndex[0]);

    // Create the render data for the triangle
    gllib::RenderData triangleData = gllib::Renderer::createRenderData(triangleVertexData, triangleVertexDataSize, triangleIndex, triangleIndexSize);

    cout << "Render loop has started!\n";
    // Loop until the user closes the window
    while (!window.getShouldClose()) {
        // Render here
        gllib::Renderer::clear();

        // Draw render data
        gllib::Renderer::drawElements(triangleData, triangleIndexSize);

        // Swap front and back buffers
        window.swapBuffers();

        // Poll for and process events
        glfwPollEvents();
    }
    cout << "Render loop ended\n";

    // Destroy render data to free vram memory
    gllib::Renderer::destroyRenderData(triangleData);
    // Destroy shader to free vram memory
    gllib::Shader::destroyShader(spSolidColor);
    // Terminate the glfw context and window
    glfwTerminate();

	return 0;
}