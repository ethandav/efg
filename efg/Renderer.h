#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include "gfx.h"
#include "gfx_window.h"
#include "gfx_imgui.h"
#include "gfx_scene.h"
#include "FlyCamera.h"
#include "Shapes.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

class Renderer
{
public:
	Renderer() {};
	~Renderer() {};

	void start(const GfxContext& gfx, const GfxWindow& window);
	void update();
	void shutdown();
private:
	GfxRef<GfxInstance> AddPrimitiveToScene(const Shapes::Types type, const char* textureFile);
	glm::mat4 CreateTransformationMatrix(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale);
	void LoadScene(const char* assetFile);
	
	const GfxContext*	m_gfx = {};
	const GfxWindow*	m_Window = nullptr;
	GfxScene	gfxScene = {};
	FlyCamera	cam = {};
	GfxArray<GfxBuffer> indexBuffers = {};
	GfxArray<GfxBuffer> vertexBuffers = {};
    GfxArray<GfxTexture> albedoBuffers;

	GfxDrawState drawState;
	GfxTexture colorBuffer = {};
	GfxTexture depthBuffer = {};

	GfxProgram litProgram = {};
	GfxKernel litKernel = {};
	GfxKernel resolveKernel = {};
	GfxSamplerState textureSampler = {};

	uint32_t instanceCount = 0;

	glm::vec3 lightPosition = glm::vec3(17.0f, 17.0f, -20.0f);
	float lightColor[3] = {1.0f, 1.0f, 1.0f};
	float lightIntensity = 1.0f;
	float specStrength = 0.0f;
	int shininess = 32;
};

#endif
