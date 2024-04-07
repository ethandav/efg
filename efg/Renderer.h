#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include "gfx.h"
#include "gfx_window.h"
#include "gfx_imgui.h"
#include "gfx_scene.h"
#include "FlyCamera.h"
#include "Shapes.h"
#include "Scene.h"

class Renderer
{
public:
	Renderer() {};
	~Renderer() {};

	void start(const GfxContext& gfx, const GfxWindow& window, const Scene& scene);
	void bindSceneBuffers();
	void update();
	void shutdown();
private:
	const GfxContext*	m_gfx = {};
	const GfxWindow*	m_Window = nullptr;
	const Scene*	m_scene = {};
	FlyCamera	cam = {};

	GfxDrawState drawState;
	GfxTexture colorBuffer = {};
	GfxTexture depthBuffer = {};

	GfxProgram litProgram = {};
	GfxKernel litKernel = {};
	GfxKernel resolveKernel = {};
	GfxSamplerState textureSampler = {};

};

#endif
