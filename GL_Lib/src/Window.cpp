#include "window.h"

using namespace GLlib;
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

GLFWwindow* Window::getReference() {
	return window;
}