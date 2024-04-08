#pragma once
#ifndef SCENE_H
#define SCENE_H

#include "gfx.h"
#include "gfx_scene.h"
#include "gfx_imgui.h"
#include "Shapes.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

class GameObject
{
public:
	GameObject() {};
	GameObject(const char* name) : name(name) {};
	GameObject(const char* name, GfxRef<GfxInstance> ref) : name(name), reference(ref) {};
	GameObject(const char* name, GfxRef<GfxInstance> ref, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale) :
		name(name), reference(ref), position(pos), rotation(rot), scale(scale) {};
	virtual ~GameObject() = default;

	const char* name = "";
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 scale = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 prevPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 prevRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 prevScale = glm::vec3(0.0f, 0.0f, 0.0f);
	GfxRef<GfxInstance> reference;
};

class Light : public GameObject
{
public:
	Light() {};
	Light(const char* name, GfxRef<GfxInstance> ref) : GameObject(name, ref) { };
	Light(const char* name, GfxRef<GfxInstance> ref, glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale) :
		lightPosition(translation),
		GameObject(name, ref, translation, rotation, scale) {}

	glm::vec3	lightPosition	= glm::vec3(0.0f, 0.0f, 0.0f);
	float		lightColor[3]	= {1.0f, 1.0f, 1.0f};
	float		lightIntensity	= 1.0f;
	float		specStrength	= 0.0f;
	int			shininess		= 32;
};

class Instance : public GameObject
{
public:
	Instance() {};
	Instance(const char* name, GfxRef<GfxInstance> ref) : GameObject(name, ref) {}
	Instance(const char* name, GfxRef<GfxInstance> ref, glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale) :
		GameObject(name, ref, translation, rotation, scale) {}
};

class Scene
{
public:
	Scene() {};
	void initialize(const GfxContext& gfx);
	void update(GfxProgram const& program) const;
	void destroy();
private:
	const GfxContext* m_gfx = nullptr;
	GfxScene gfxScene = {};

	void AddPrimitive(const char* name, const Shapes::Types type, const char* textureFile,
		glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));
	void addLight(const char* name,
		glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)
	);
	glm::mat4 CreateTransformationMatrix(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale) const;
	void LoadSceneFromFile(const char* assetFile);

	GfxArray<GfxBuffer> indexBuffers = {};
	GfxArray<GfxBuffer> vertexBuffers = {};
    GfxArray<GfxTexture> albedoBuffers;

	std::vector<Light*> lights = {};
	std::vector<GameObject*> gameObjects = {};
};

#endif SCENE_H
