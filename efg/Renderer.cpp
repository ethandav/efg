#include "Renderer.h"

void Renderer::initialize(const GfxWindow& window)
{
	float time = 0.0f;
	m_Window = &window;
	gfx = gfxCreateContext(window);

	std::for_each(renderLayers.begin(), renderLayers.end(), [&](RenderLayer* layer) {
		layer->initialize(gfx);
	});
}

void Renderer::attachRenderLayer(RenderLayer* layer)
{
	renderLayers.push_back(layer);
}

void Renderer::update(double deltaTime)
{
	std::for_each(renderLayers.begin(), renderLayers.end(), [&](RenderLayer* layer) {
		layer->update(gfx, *m_Window, deltaTime);
	});

	gfxFrame(gfx);
}

void Renderer::shutdown()
{
	std::for_each(renderLayers.begin(), renderLayers.end(), [&](RenderLayer* layer) {
		layer->destroy(gfx);
		delete layer;
	});

	gfxDestroyContext(gfx);
}
