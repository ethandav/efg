#pragma once
#ifndef SCENE_H
#define SCENE_H

#include "RenderLayer.h"
#include "Shapes.h"
#include "FlyCamera.h"
#include "GameObjects.h"
#include "Light.h"

class Scene : public RenderLayer
{
public:
	Scene() {};

	Mesh* AddPrimitive(GfxContext const& gfx, const char* name, const Shapes::Types type, bool atCam = true,
		const char* textureFile = nullptr, const char* specMap = nullptr,
		glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));
	void addDirectionalLight(GfxContext const& gfx);

	std::vector<GameObject*>* getGameObjects();
	std::vector<Light*>* getSceneLights();

private:
	GfxScene gfxScene = {};

	virtual void initialize(const GfxContext& gfx);
	virtual void update(GfxContext const& gfx, GfxWindow const& window);
	virtual void destroy(GfxContext const& gfx);

	void loadScene(GfxContext const& gfx);
	void updateLights(GfxContext const& gfx);
	void updateGameObjects(GfxContext const& gfx);
	void DrawInstanced(GfxContext const& gfx, GameObject* obj);
	void updateSkybox(GfxContext const& gfx);
	void LoadSceneFromFile(GfxContext const& gfx, const char* assetFile);
	void createSkybox(GfxContext const& gfx, const char* textureFiles[6]);

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
	std::vector<Light*>			sceneLights				= {};
	Skybox*						skybox					= nullptr;
};

#endif SCENE_H
