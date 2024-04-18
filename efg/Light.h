#pragma once
#include "RenderLayer.h"

struct DirectionalInput
{
	glm::vec3 direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	glm::vec3 ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
};

class Light
{
public:
	Light() {};
	virtual void update(GfxContext const& gfx, GfxProgram const& program) = 0;
	virtual void gui() = 0;
	virtual void destroy(GfxContext const& gfx) = 0;

	const char* name = nullptr;
	glm::vec3 color = glm::vec3(1.0f);
	glm::vec3 ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
};

class Directional : public Light
{
public:
	virtual void update(GfxContext const& gfx, GfxProgram const& program);
	virtual void gui();
	virtual void destroy(GfxContext const& gfx);
private:
	DirectionalInput input = {};
	GfxBuffer buffer	= {};
};

class Point : public Light
{
public:
	virtual void update(GfxContext const& gfx, GfxProgram const& program);
	virtual void gui();
	virtual void destroy(GfxContext const& gfx);
private:
	DirectionalInput input = {};
	GfxBuffer* buffer	= nullptr;
};
