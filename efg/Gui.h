#pragma once
#ifndef GUI_H
#define GUI_H

#include "RenderLayer.h"
#include "GameObjects.h"

class Gui : public RenderLayer
{
public:
	Gui() {};
	Gui(const std::vector<GameObject*>* gameObjects) : gameObjects(gameObjects) {};
	~Gui() {};
	virtual void initialize(const GfxContext& gfx);
	virtual void update(GfxContext const& gfx, GfxWindow const& window);
	virtual void destroy(GfxContext const& gfx);
private:
	const std::vector<GameObject*>* gameObjects;
};

#endif // GUI_H
