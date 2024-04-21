#include "Engine.h"

#ifdef FPS_COUNTER
#include <chrono>
#include <iostream>
#endif

void Engine::Initialize()
{
	window = gfxCreateWindow(1920, 1080, "efg");

	Scene* scene = new Scene();
	Gui* gui = new Gui(scene);
	renderer.attachRenderLayer(scene);
	renderer.attachRenderLayer(gui);
	renderer.initialize(window);
}

void Engine::start()
{
#ifdef FPS_COUNTER
	std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
	int frameCount = 0;
	double fps = 0.0;
#endif

	while (!gfxWindowIsCloseRequested(window))
	{
		gfxWindowPumpEvents(window);
		renderer.update();
#ifdef FPS_COUNTER
		frameCount++;

		auto currentTime = std::chrono::steady_clock::now();
		double timeDifference = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count() / 1000.0;
		
		if (timeDifference >= 1.0) {
		    fps = frameCount / timeDifference;
		    frameCount = 0;
		    lastTime = currentTime;
		    std::cout << "FPS: " << fps << std::endl;
		}
#endif
	}
}

void Engine::shutdown()
{
	renderer.shutdown();
	gfxDestroyWindow(window);
}
