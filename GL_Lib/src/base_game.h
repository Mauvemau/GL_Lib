#include "deps.h"

#include "core.h"
#include "window.h"
#include "renderer.h"
#include "lib_time.h"

#include "triangle.h"

namespace gllib {

	class DLLExport BaseGame {
	private:
		LibCore libCore;

		bool initInternal();
		void updateInternal();
		void uninitInternal();

	protected:
		Window* window;

		virtual void init() {};
		virtual void update() {};
		virtual void uninit() {};

	public:
		BaseGame();
		virtual ~BaseGame();

		void start();
	};

}