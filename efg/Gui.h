#pragma once
#ifndef GUI_H
#define GUI_H

#include "RenderLayer.h"
#include "Scene.h"

class Gui : public RenderLayer
{
public:
	Gui(Scene* scene) : scene(scene) {};
	~Gui() {};
	virtual void initialize(const GfxContext& gfx);
	virtual void update(GfxContext const& gfx, GfxWindow const& window);
	virtual void destroy(GfxContext const& gfx);
private:
	Scene* scene = nullptr;
};

#endif // GUI_H
