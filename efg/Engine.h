#pragma once
#ifndef ENGINE_H
#define  ENGINE_H

#include "Renderer.h"
#include "Scene.h"
#include "Gui.h"

class Engine
{
public:
	Engine() {};
	~Engine() {};

	void Initialize();
	void start();
	void shutdown();
private:
	Renderer renderer = {};
	GfxContext gfx = {};
	GfxWindow window = {};
};

#endif ENGINE_H
