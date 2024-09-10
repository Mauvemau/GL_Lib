#include "base_game.h"

#include "shader.h"

#include <iostream>

using namespace gllib;
using namespace std;

// Constr/Destr

BaseGame::BaseGame() {
	window = new Window(640, 480, "Loading...");

	// Confirm that the window has been properly initialized
	if (!window->getIsInitialized()) {
		cout << "Failed to create window!\n";
		glfwTerminate();
		return;
	}
	// Make the window's context current
	window->makeContextCurrent();

	// Initialize the GLAD library
	gllib::LibCore::initGlad();
}

BaseGame::~BaseGame() {
	delete window;
}

// Private

bool BaseGame::initInternal() {
	// Check if the PC has a working OpenGL driver
	cout << glGetString(GL_VERSION) << "\n";

	// Load vertex and fragment shaders from files
	const char* vertexSource = gllib::Shader::loadShader("solidColorV.glsl");
	const char* fragmentSource = gllib::Shader::loadShader("solidColorF.glsl");
	// Create shader program
	unsigned int spSolidColor = gllib::Shader::createShader(vertexSource, fragmentSource);
	// Set current shader program
	gllib::Shader::useShaderProgram(spSolidColor);

	init();
	updateInternal();
	Shader::destroyShader(spSolidColor);
	return true;
}

void BaseGame::updateInternal() {
	// Loop until the user closes the window
	while (!window->getShouldClose()) {

		update();

		// Swap front and back buffers
		window->swapBuffers();
		// Poll for and process events
		LibCore::pollEvents();
	}
}

void BaseGame::uninitInternal() {
	uninit();
}

// Public

void BaseGame::start() {
	initInternal();
	uninitInternal();
}