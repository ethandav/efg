#pragma once
#include "RenderLayer.h"

class GameObject
{
public:
	GameObject() {};
	GameObject(const char* name) : name(name) {};
	GameObject(const char* name, GfxRef<GfxInstance> ref) : name(name), reference(ref) {};
	GameObject(const char* name, GfxRef<GfxInstance> ref, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale) :
		name(name), reference(ref), position(pos), rotation(rot), scale(scale) {};
	virtual ~GameObject() = default;

	const char*				name			= "";
	GfxRef<GfxInstance>		reference		= {};
	glm::vec3				position		= glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3				rotation		= glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3				scale			= glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3				prevPosition	= glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3				prevRotation	= glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3				prevScale		= glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4				modelMatrix		= glm::mat4();
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
	float		lightIntensity	= 0.0f;
	float		specStrength	= .5f;
	int			shininess		= 32;
};

class Mesh: public GameObject
{
public:
	Mesh() {};
	Mesh(const char* name, GfxRef<GfxInstance> ref) : GameObject(name, ref) {}
	Mesh(const char* name, GfxRef<GfxInstance> ref, glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale) :
		GameObject(name, ref, translation, rotation, scale) {}
};

class Skybox
{
public:
	Skybox() {};
	Skybox(GfxRef<GfxMesh> meshRef, GfxRef<GfxMaterial> matRef) : mesh(meshRef), material(matRef) {};
	~Skybox() {};

	GfxRef<GfxMesh> mesh = {};
	GfxRef<GfxMaterial> material = {};
};

glm::mat4 CreateModelMatrix(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale);
