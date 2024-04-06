#pragma once
#ifndef ENGINE_H
#define  ENGINE_H

#include "Renderer.h"

class Engine
{
public:
	Engine() {};
	~Engine() {};

	void Initialize();
	void start();
	void shutdown();
private:
	Renderer renderer;
	GfxWindow window = {};
};

#endif ENGINE_H
