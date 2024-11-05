#include "deps.h"

#include "core.h"
#include "Input.h"
#include "window.h"
#include "renderer.h"
#include "lib_time.h"
#include "loader.h"

#include "triangle.h"
#include "rectangle.h"
#include "sprite.h"
#include "animation.h"
#include "shader.h"

namespace gllib {

	class DLLExport BaseGame {
	private:
		LibCore libCore;
		Input* input;
		
		bool initInternal();
		void updateInternal();
		void uninitInternal();

	protected:
		Window* window;

		unsigned int shaderProgramSolidColor;
		unsigned int shaderProgramTexture;

		virtual void init() {};
		virtual void update() {};
		virtual void uninit() {};

	public:
		BaseGame();
		virtual ~BaseGame();

		void start();
	};

}