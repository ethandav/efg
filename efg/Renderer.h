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
	GfxContext					gfx				= {};
	std::vector<RenderLayer*>	renderLayers	= {};
	const GfxWindow*			m_Window		= nullptr;
};

#endif
