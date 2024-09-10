#pragma once
#include "deps.h"

#include <glm.hpp>

namespace gllib {

	/// <summary>
	/// This struct contains the 3 basic buffers needed to draw using glDrawElements
	/// </summary>
	struct RenderData {
		unsigned int VAO; // Vertex Array Object
		unsigned int VBO; // Vertex Buffer Object
		unsigned int EBO; // Element Buffer Object
	};

	/// <summary>
	/// Fully static class
	/// </summary>
	class DLLExport Renderer {
	private:
		static glm::mat4 projMatrix;
		static glm::mat4 viewMatrix;
		static glm::mat4 modelMatrix;

	public:
		static void setUpVertexAttributes();

		static unsigned int createVertexArrayObject();
		static unsigned int createVertexBufferObject(const float vertexData[], GLsizei bufferSize);
		static unsigned int createElementBufferObject(const int index[], GLsizei bufferSize);

		static RenderData createRenderData(const float vertexData[], GLsizei vertexDataSize, const int index[], GLsizei indexSize);
		static void destroyRenderData(RenderData rData);

		static void drawElements(RenderData rData, GLsizei indexSize);

		static void setModelMatrix(glm::mat4 newModelMatrix);

		static void clear();
	};

}