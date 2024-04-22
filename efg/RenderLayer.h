#pragma once
#ifndef RENDERLAYER_H
#define RENDERLAYER_H

#include "gfx.h"
#include "gfx_window.h"
#include "gfx_scene.h"
#include "gfx_imgui.h"

class RenderLayer
{
public:
	RenderLayer() {};
	virtual ~RenderLayer() = default;
	virtual void initialize(GfxContext const& gfx) = 0;
	virtual void update(GfxContext const& gfx, GfxWindow const& window, double deltaTime) = 0;
	virtual void destroy(GfxContext const& gfx) = 0;
private:
};

#endif //RENDERLAYER_H
