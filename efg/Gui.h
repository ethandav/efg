#pragma once

#include "Renderer.h"

class Gui : public RenderLayer
{
public:
	Gui() {};
	~Gui() {};
	virtual void initialize(const GfxContext& gfx);
	virtual void update(GfxContext const& gfx, GfxProgram const& program);
	virtual void destroy(GfxContext const& gfx);
private:
};
