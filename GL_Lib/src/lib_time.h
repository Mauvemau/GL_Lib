#pragma once

namespace gllib {

	class LibTime {
	private:
		static double previousFrameTime;
		static double currentFrameTime;
		static double fpsUpdateTime;
		static int frameCount;
		static double fps;
	public:
		static int getFPS();
		static double getDeltaTime();
		static double getElapsedTime();
		static void updateFrameTime();
	};

}