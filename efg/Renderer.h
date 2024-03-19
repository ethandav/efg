#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include "gfx.h"
#include "gfx_window.h"
#include "gfx_imgui.h"
#include "gfx_scene.h"
#include "FlyCamera.h"
#include "Shapes.h"

#include <glm/gtx/euler_angles.hpp>

class Renderer
{
public:
	Renderer() {};
	~Renderer() {};

	void start(GfxWindow window);
	void shutdown();
private:
	GfxRef<GfxInstance> AddPrimitiveToScene(const Shapes::Types type, const char* textureFile);
	glm::mat4 CreateTransformationMatrix(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale);
	void LoadScene(const char* assetFile);
	
	GfxContext	gfx = {};
	GfxWindow*	m_Window = nullptr;
	GfxScene	gfxScene = {};
	FlyCamera	cam = {};
	GfxArray<GfxBuffer> indexBuffers = {};
	GfxArray<GfxBuffer> vertexBuffers = {};
    GfxArray<GfxTexture> albedoBuffers;
};

#endif
