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

		void updateRenderData(Color color);
	public:
		Sprite(Vector3 translation, Vector3 rotation, Vector3 scale, Color color);
		Sprite(Transform transform, Color color);
		virtual ~Sprite() override;

		Color getColor();

		void setColor(Color color);

		void addTexture(std::string path);

		virtual void draw() override;
	};
}