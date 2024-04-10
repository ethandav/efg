#pragma once
#ifndef GUI_H
#define GUI_H

#include "RenderLayer.h"

class Gui : public RenderLayer
{
public:
	Gui() {};
	~Gui() {};
	virtual void initialize(const GfxContext& gfx);
	virtual void update(GfxContext const& gfx, GfxWindow const& window);
	virtual void destroy(GfxContext const& gfx);
private:
};

#endif // GUI_H
