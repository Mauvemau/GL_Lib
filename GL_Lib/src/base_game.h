#pragma once
#include "deps.h"

#include "core.h"
#include "input.h"
#include "window.h"
#include "renderer.h"
#include "lib_time.h"
#include "loader.h"
#include "camera/camera.h"

#include "primitives/triangle.h"
#include "primitives/rectangle.h"
#include "primitives/sprite.h"
#include "primitives/box.h"
#include "primitives/animation.h"
#include "models/mesh_group.h"
#include "models/model.h"
#include "models/model_importer.h"
#include "shader.h"

namespace gllib {

	class DLLExport BaseGame {
	private:
		LibCore libCore;

		bool shouldStop = false;
		
		bool initInternal();
		void updateInternal();
		void uninitInternal();

	protected:
		Window* window;
		Input* input;

		unsigned int shaderProgramSolidColor;
		unsigned int shaderProgramSolidColorLit;
		unsigned int shaderProgramTexture;
		unsigned int shaderProgramNormals;

		virtual void init() {};
		virtual void update() {};
		virtual void uninit() {};

	public:
		BaseGame();
		virtual ~BaseGame();

		void start();
		void stop();
	};

}
