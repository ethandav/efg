#pragma once
#ifndef SCENE_H
#define SCENE_H

#include "gfx.h"
#include "gfx_scene.h"
#include "Shapes.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

class Scene
{
public:
	Scene();
	void initialize(const GfxContext& gfx);
	void update(GfxProgram const& program) const;
	void destroy();
private:
	const GfxContext* m_gfx = nullptr;
	GfxScene gfxScene = {};

	GfxRef<GfxInstance> AddPrimitiveToScene(const Shapes::Types type, const char* textureFile);
	glm::mat4 CreateTransformationMatrix(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale);
	void LoadScene(const char* assetFile);

	GfxArray<GfxBuffer> indexBuffers = {};
	GfxArray<GfxBuffer> vertexBuffers = {};
    GfxArray<GfxTexture> albedoBuffers;
};

#endif SCENE_H
