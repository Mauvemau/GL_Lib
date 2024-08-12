#pragma once

#include "deps.h"

namespace gllib {

	/// <summary>
	/// This struct contains the 3 basic buffers needed to draw using glDrawElements
	/// </summary>
	struct RenderData {
		unsigned int VAO; // Vertex Array Object ()
		unsigned int VBO; // Vertex Buffer Object
		unsigned int EBO; // Element Buffer Object
	};

	/// <summary>
	/// Fully static class
	/// </summary>
	class DLLExport Renderer {
	public:
		static void setUpVertexAttributes();

		static unsigned int createVertexArrayObject();
		static unsigned int createVertexBufferObject(const float vertexData[], size_t bufferSize);
		static unsigned int createElementBufferObject(const int index[], size_t bufferSize);

		static RenderData createRenderData(const float vertexData[], size_t vertexDataSize, const int index[], size_t indexSize);
		static void destroyRenderData(RenderData rData);

		static void drawElements(RenderData rData, size_t indexSize);

		static void clear();
	};

}