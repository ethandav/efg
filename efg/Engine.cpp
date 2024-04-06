#include "Engine.h"

void Engine::Initialize()
{
	window = gfxCreateWindow(1280, 720, "Freeside Engine");
	gfx = gfxCreateContext(window);
	renderer.start(gfx, window);
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
