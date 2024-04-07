#include "Engine.h"

void Engine::Initialize()
{
	window = gfxCreateWindow(1920, 1080, "Freeside Engine");
	gfx = gfxCreateContext(window);
	scene.initialize(gfx);

	renderer.start(gfx, window, scene);
}

void Engine::start()
{
	while (!gfxWindowIsCloseRequested(window))
	{
		gfxWindowPumpEvents(window);
		renderer.update();
	}
}

void Engine::shutdown()
{
	scene.destroy();
	renderer.shutdown();
	gfxDestroyWindow(window);
}
