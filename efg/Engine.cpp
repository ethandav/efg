#include "Engine.h"
#include "Renderer.h"

void Engine::Initialize()
{
	GfxWindow window = gfxCreateWindow(1280, 720, "Freeside Engine");
	Renderer renderer;

	renderer.start(window);
	
	while (!gfxWindowIsCloseRequested(window))
	{
		renderer.update();
	}

	renderer.shutdown();
}
