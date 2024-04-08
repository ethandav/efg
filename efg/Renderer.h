#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include "gfx.h"
#include "gfx_window.h"
#include "gfx_imgui.h"
#include "gfx_scene.h"
#include "FlyCamera.h"
#include "Shapes.h"

class RenderLayer
{
public:
	RenderLayer() {};
	virtual ~RenderLayer() = default;
	virtual void initialize(GfxContext const& gfx) = 0;
	virtual void update(GfxContext const& gfx, GfxProgram const& program) = 0;
	virtual void destroy(GfxContext const& gfx) = 0;
private:
};

class Renderer
{
public:
	Renderer() {};
	~Renderer() {};

	void initialize(const GfxContext& gfx, const GfxWindow& window);
	void attachRenderLayer(RenderLayer* layer);
	void update();
	void shutdown();
private:
	const GfxContext*	m_gfx = {};
	const GfxWindow*	m_Window = nullptr;
	FlyCamera	cam = {};

	GfxDrawState drawState;
	GfxTexture colorBuffer = {};
	GfxTexture depthBuffer = {};

	GfxProgram litProgram = {};
	GfxKernel litKernel = {};
	GfxKernel resolveKernel = {};
	GfxSamplerState textureSampler = {};

	std::vector<RenderLayer*> renderLayers = {};

};

#endif
