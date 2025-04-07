#pragma once
#include "deps.h"

#ifdef _WIN32 // Directory is different in linux
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#else
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#endif // _WIN32

namespace gllib {

	/// <summary>
	/// This struct contains the 3 basic buffers needed to draw using glDrawElements
	/// </summary>
	struct DLLExport RenderData {
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
		static void setUpMVP();

		static unsigned int createVertexArrayObject();
		static unsigned int createVertexBufferObject(const float vertexData[], GLsizei bufferSize);
		static unsigned int createElementBufferObject(const int index[], GLsizei bufferSize);

		static RenderData createRenderData(const float vertexData[], GLsizei vertexDataSize, const int index[], GLsizei indexSize);
		static void destroyRenderData(RenderData rData);

		static void drawElements(RenderData rData, GLsizei indexSize);
		static void drawTexture(RenderData rData, GLsizei indexSize, unsigned int textureID);

		static void bindTexture(unsigned int textureID);
		static void getTextureSize(unsigned int textureID, int* width, int* height);

		static void setModelMatrix(glm::mat4 newModelMatrix);
		static void setOrthoProjectionMatrix(float width, float height);
		static void setPerspectiveProjectionMatrix(float width, float height, float fov, float farPlane);

		static void clear();
	};

}
