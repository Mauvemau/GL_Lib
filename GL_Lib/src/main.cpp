/*
#include "core.h"
#include "window.h"
#include "renderer.h"
#include "shader.h"
#include "lib_time.h"

#include "triangle.h"
*/
#include "base_game.h"

#include <iostream>

using namespace std;

class Game : public gllib::BaseGame {
protected:
    void init() override;
    void update() override;
    void uninit() override;
public:
    Game();
    ~Game() override;
};

Game::Game() {
    cout << "Game created!\n";
}

Game::~Game() {
    cout << "Game destroyed!\n";
}

void Game::init() {
    cout << "External init!!!!\n";
}

void Game::update() {

}

void Game::uninit() {
    cout << "External uninit!!!\n";
}

int main() {
    Game game;
    game.start();
}

/*
int main() {
    // Initialize the GLFW library
    gllib::LibCore libCore;

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
    gllib::LibCore::initGlad();

    // Check if the PC has a working OpenGL driver
    cout << glGetString(GL_VERSION) << "\n";

    // Load vertex and fragment shaders from files
    const char* vertexSource = gllib::Shader::loadShader("solidColorV.glsl");
    const char* fragmentSource = gllib::Shader::loadShader("solidColorF.glsl");
    // Create shader program
    unsigned int spSolidColor = gllib::Shader::createShader(vertexSource, fragmentSource);
    // Set current shader program
    gllib::Shader::useShaderProgram(spSolidColor);

    // Create a triangle
    gllib::Triangle triangle;

    cout << "Render loop has started!\n";
    // Loop until the user closes the window
    while (!window.getShouldClose()) {
        // Render here
        gllib::Renderer::clear();

        //cout << "FPS: " << gllib::LibTime::getFPS() << ", FrameTime: " << gllib::LibTime::getDeltaTime() << ".\n";

        // Draw the triangle
        triangle.draw();

        // Swap front and back buffers
        window.swapBuffers();

        // Poll for and process events
        gllib::LibCore::pollEvents();
    }
    cout << "Render loop ended\n";

    // Destroy shader to free vram memory
    gllib::Shader::destroyShader(spSolidColor);
    return 0;
}
*/
