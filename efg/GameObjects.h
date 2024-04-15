#pragma once
#include "RenderLayer.h"

struct MaterialBuffer
{
	struct Material
	{
		glm::vec4 ambient = glm::vec4(0.2f);
		glm::vec4 diffuse = glm::vec4(0.5f);
		glm::vec4 specular = glm::vec4(1.0f);
		float shininess = 32.0f;
	};
	Material material;
};

struct LightBuffer
{
	struct Light
	{
		glm::vec4	position		= glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		glm::vec4	specular		= glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		glm::vec4	ambientColor	= glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		glm::vec4	diffuseColor	= glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	};
	Light light;
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
	virtual void destroy(GfxContext const& gfx) = 0;

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

class LightObject : public GameObject
{
public:
	LightObject() {};
	LightObject(const char* name) : GameObject(name) { };
	LightObject(const char* name, glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale) :
		GameObject(name, translation, rotation, scale) {}
	virtual void draw(GfxContext const& gfx, GfxProgram const& prgoram);
	virtual void gui();
	virtual void destroy(GfxContext const& gfx);

	glm::vec3 position	= glm::vec3(0.0f);
	glm::vec3 ambient	= glm::vec3(1.0f);
	glm::vec3 diffuse	= glm::vec3(0.5f);
	glm::vec3 specular	= glm::vec3(1.0f);
	glm::vec3 color		= glm::vec3(1.0f);

private:
	LightBuffer lightBuffer;

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
	virtual void destroy(GfxContext const& gfx);

	MaterialBuffer materialBuffer = {};
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
	virtual void draw(GfxContext const& gfx, GfxProgram const& program);
	virtual void gui();
	virtual void destroy(GfxContext const& gfx);

	MaterialBuffer materialBuffer = {};

};

class Skybox : public Mesh
{
public:
	Skybox() {};
	~Skybox() {};
};

glm::mat4 CreateModelMatrix(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale);
