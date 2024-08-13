#pragma once

namespace gllib {

	class LibCore {
	public:
		LibCore();
		~LibCore();

		/// <summary>
		/// Can only be called after initializing a window and setting up the glfw context
		/// </summary>
		static bool initGlad();
		static void pollEvents();
	};

}