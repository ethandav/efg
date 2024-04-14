#pragma once
#include "RenderLayer.h"

struct Properties
{
	glm::vec4 ambient = glm::vec4(0.0f);
	glm::vec4 diffuse = glm::vec4(0.0f);
	glm::vec4 specular = glm::vec4(0.0f);
	float shininess = 0.0;
};

struct Material
{
	Properties properties;
};

class GameObject
{
public:
	GameObject() {};
	GameObject(const char* name) : name(name) {};
	GameObject(const char* name, GfxRef<GfxInstance> ref) : name(name), reference(ref) {};
	GameObject(const char* name, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale) :
		name(name), position(pos), rotation(rot), scale(scale) {};
	GameObject(const char* name, GfxRef<GfxInstance> ref, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale) :
		name(name), reference(ref), position(pos), rotation(rot), scale(scale) {};
	virtual ~GameObject() = default;
	virtual void draw(GfxContext const& gfx, GfxProgram const& prgoram) = 0;
	virtual void gui() = 0;

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
	Light(const char* name) : GameObject(name) { };
	Light(const char* name, glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale) :
		lightPosition(translation),
		GameObject(name, translation, rotation, scale) {}
	virtual void draw(GfxContext const& gfx, GfxProgram const& prgoram);
	virtual void gui();

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
	Mesh(const char* name) : GameObject(name) {}
	Mesh(const char* name, glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale) :
		GameObject(name, translation, rotation, scale) {}
	virtual void draw(GfxContext const& gfx, GfxProgram const& prgoram);
	virtual void gui();

	Material material = {};
	bool hasTexture = false;
	GfxTexture texture = {};
    std::vector<GfxVertex>  vertices        = {};
    std::vector<uint32_t>   indices         = {};
	GfxBuffer indexBuffer = {};
	GfxBuffer vertexBuffer = {};
};

class Instanced : public GameObject
{
public:
	Instanced() {};
	Instanced(const char* name, GfxRef<GfxInstance> ref) : GameObject(name, ref) {}
	Instanced(const char* name, GfxRef<GfxInstance> ref, glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale) :
		GameObject(name, ref, translation, rotation, scale) {}
	virtual void draw(GfxContext const& gfx, GfxProgram const& program) {};
	virtual void gui();

};

class Skybox : public Mesh
{
public:
	Skybox() {};
	~Skybox() {};
};

glm::mat4 CreateModelMatrix(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale);
