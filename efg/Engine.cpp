#include "Engine.h"

void Engine::Initialize()
{
	window = gfxCreateWindow(1280, 720, "Freeside Engine");
	gfx = gfxCreateContext(window);
	scene.initialize(gfx);

	renderer.start(gfx, window, scene);
}

void Engine::start()
{
	while (!gfxWindowIsCloseRequested(window))
	{
		renderer.update();
	}
}

void Engine::shutdown()
{
	renderer.shutdown();
	gfxDestroyWindow(window);
}
