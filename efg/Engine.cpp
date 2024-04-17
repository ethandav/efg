#include "Engine.h"

void Engine::Initialize()
{
	window = gfxCreateWindow(1280, 720, "efg");

	Scene* scene = new Scene();
	Gui* gui = new Gui(scene);
	renderer.attachRenderLayer(scene);
	renderer.attachRenderLayer(gui);
	renderer.initialize(window);
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
