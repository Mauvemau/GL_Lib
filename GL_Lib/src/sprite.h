#pragma once

#include "shape.h"

#include <iostream>
#include <vector>

namespace gllib {
	struct UVCoords {
		float u, v;
	};

	struct Frame {
		unsigned int textureID;
		UVCoords uvCoords[4];
	};

	class DLLExport Sprite : public Shape {
	private:
		std::vector<Frame> textures;
		Color color;
		int currentTexture;

		void updateRenderData();
	public:
		Sprite(Vector3 translation, Vector3 rotation, Vector3 scale, Color color);
		Sprite(Transform transform, Color color);
		virtual ~Sprite() override;

		Color getColor();

		void setColor(Color color);
		void setCurrentTexture(unsigned int index);
		void setNextTexture();
		std::vector<Frame> getTextures() const;

		void addFrame(Frame frame);
		void addTexture(unsigned int textureID);
		void addTexture(std::string path);
		void addTexture(unsigned int textureID, int offsetX, int offsetY, int width, int height);

		virtual void draw() override;
	};
}