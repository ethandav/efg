#pragma once
#ifndef SCENE_H
#define SCENE_H

#include "RenderLayer.h"
#include "Shapes.h"
#include "FlyCamera.h"
#include "GameObjects.h"


class Scene : public RenderLayer
{
public:
	Scene() {};
	virtual void initialize(const GfxContext& gfx);
	virtual void update(GfxContext const& gfx, GfxWindow const& window);
	virtual void destroy(GfxContext const& gfx);

	const std::vector<GameObject*>* getGameObjects();

private:
	GfxScene gfxScene = {};

	void loadScene(GfxContext const& gfx);
	void updateGameObjects(GfxContext const& gfx);
	void DrawInstanced(GfxContext const& gfx, GameObject* obj);
	void updateSkybox(GfxContext const& gfx);
	Mesh* AddPrimitive(GfxContext const& gfx, const char* name,
		const Shapes::Types type, const char* textureFile = nullptr,
		glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));
	LightObject* addLight(GfxContext const& gfx, const char* name,
		glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)
	);
	void LoadSceneFromFile(GfxContext const& gfx, const char* assetFile);
	void createSkybox(GfxContext const& gfx, const char* textureFile);

	FlyCamera					cam						= {};
	GfxDrawState				drawState				= {};
	GfxTexture					colorBuffer				= {};
	GfxTexture					depthBuffer				= {};
	GfxProgram					litProgram				= {};
	GfxKernel					litKernel				= {};
	GfxProgram					skyboxProgram			= {};
	GfxKernel					skyboxKernel			= {};
	GfxKernel					litResolveKernel		= {};
	GfxKernel					skyboxResolveKernel		= {};
	GfxSamplerState				textureSampler			= {};
	GfxArray<GfxBuffer>			indexBuffers			= {};
	GfxArray<GfxBuffer>			vertexBuffers			= {};
	GfxArray<GfxTexture>		albedoBuffers			= {};
	std::vector<GameObject*>	gameObjects				= {};
	Skybox*						skybox					= nullptr;
};

#endif SCENE_H
