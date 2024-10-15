#pragma once

#include "shape.h"

namespace gllib {
	class DLLExport Sprite : public Shape {
	private:
		Color color;

		void updateRenderData(Color color);
	public:
		Sprite(Vector3 translation, Vector3 rotation, Vector3 scale, Color color);
		Sprite(Transform transform, Color color);
		virtual ~Sprite() override;

		Color getColor();

		void setColor(Color color);

		virtual void draw() override;
	};
}