#pragma once
#include "deps.h"

#include <iostream>

namespace gllib {
	class Shader {
	private:
		static std::string getShaderType(unsigned int type);
		static unsigned int compileShader(unsigned int type, std::string source);
	public:
		static unsigned int createShader(const char* vertexShader, const char* fragmentShader);
		static void destroyShader(unsigned int program);
		static const char* loadShader(std::string filePath);
		static void useShaderProgram(unsigned int shaderProgram);
	};
};