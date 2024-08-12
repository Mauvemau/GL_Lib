#pragma once

#include "deps.h"

#include <iostream>

namespace gllib {

	class DLLExport Window {
	private:
		GLFWwindow* window;
	public:
		/// <summary>
		/// The constructor creates a window and opens it
		/// </summary>
		Window(int width, int height, std::string title);
		/// <summary>
		/// Destructor simply closes the window the function glfwTerminate should take care of uninitializing it later
		/// </summary>
		~Window();

		/// <summary>
		/// Links the window to the current glfw context
		/// </summary>
		void makeContextCurrent();
		/// <summary>
		/// Swaps render buffers
		/// </summary>
		void swapBuffers();

		/// <summary>
		/// Returns the variable window
		/// </summary>
		GLFWwindow* getReference();
		/// <summary>
		/// Used to confirm if the window has been properly initialized
		/// </summary>
		/// <returns></returns>
		bool getIsInitialized();
		/// <summary>
		/// Returns if the user has tried to close the window manually
		/// </summary>
		bool getShouldClose();
	};

}