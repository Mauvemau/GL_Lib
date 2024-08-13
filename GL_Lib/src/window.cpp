#include "window.h"

using namespace gllib;
using namespace std;

Window::Window(int width, int height, string title) {
	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	cout << "Window created!\n";
}

Window::~Window() {
	// We just close it, we will be using glfwTerminate to clean up the resources.
	glfwSetWindowShouldClose(window, true);
	cout << "Window closed.\n";
}

// Public

void Window::makeContextCurrent() {
	glfwMakeContextCurrent(window);
}

void Window::swapBuffers() {
	glfwSwapBuffers(window);
}

GLFWwindow* Window::getReference() {
	return window;
}

bool Window::getIsInitialized() {
	return (window);
}

bool Window::getShouldClose() {
	return glfwWindowShouldClose(window);
}
