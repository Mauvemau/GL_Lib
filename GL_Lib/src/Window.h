#pragma once

#include "deps.h"

#include <iostream>

namespace GLlib {

	class DLLExport Window {
	private:
		GLFWwindow* window;
	public:
		Window(int width, int height, std::string title);
		~Window();

		GLFWwindow* getReference();
	};

}