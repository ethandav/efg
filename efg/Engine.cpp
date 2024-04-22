#include "Engine.h"

#include <chrono>

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
	std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
	int frameCount = 0;
	double fps = 0.0;

	while (!gfxWindowIsCloseRequested(window))
	{
		auto currentTime = std::chrono::steady_clock::now();
		double deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count() / 1000.0;
		lastTime = currentTime;

		gfxWindowPumpEvents(window);
		renderer.update(deltaTime);
		frameCount++;

		if (deltaTime >= 1.0) {
		    fps = frameCount / deltaTime;
		    frameCount = 0;
		}
	}
}

void Engine::shutdown()
{
	renderer.shutdown();
	gfxDestroyWindow(window);
}
