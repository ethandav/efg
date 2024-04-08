#include "Engine.h"

void Engine::Initialize()
{
	window = gfxCreateWindow(1920, 1080, "Freeside Engine");
	gfx = gfxCreateContext(window);

	Scene* scene = new Scene();
	Gui* gui = new Gui();
	renderer.attachRenderLayer(scene);
	renderer.attachRenderLayer(gui);

	renderer.initialize(gfx, window);
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
	renderer.shutdown();
	gfxDestroyWindow(window);
}
