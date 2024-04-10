#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include "RenderLayer.h"

class Renderer
{
public:
	Renderer() {};
	~Renderer() {};

	void initialize(const GfxWindow& window);
	void attachRenderLayer(RenderLayer* layer);
	void update();
	void shutdown();
private:
	GfxContext gfx = {};
	const GfxWindow*	m_Window = nullptr;

	std::vector<RenderLayer*> renderLayers = {};

};

#endif
