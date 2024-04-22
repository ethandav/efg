#pragma once
#include "RenderLayer.h"
#include "Animation.h"

class Light;

struct DirectionalInput
{
	glm::vec3 direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	glm::vec3 ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
};

struct PointInput
{
	glm::vec3 position = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
	float constant = 1.0f;
	float pLinear = 0.009f;
	float quadratic = 0.0032f;
};

class LightingManager
{
public:
	LightingManager() {};
	void initialize(GfxContext const& gfx);
	void addDirectionalLight(GfxContext const& gfx);
	Light* addPointLight(GfxContext const& gfx, glm::vec3 const* position = nullptr,
		glm::vec3 const* color = nullptr);
	void update(GfxContext const& gfx, GfxProgram const& program, double deltaTime);
	void destroy(GfxContext const& gfx);
	std::vector<Light*> const* getLights();

	bool updateDirectionals = false;
	bool updatePoints = false;
private:
	std::vector<Light*> lights = {};
	GfxBuffer directionalLightsBuffer = {};
	GfxBuffer pointLightsBuffer = {};
	std::vector<DirectionalInput*> directionalLights = {};
	std::vector<PointInput*> pointLights = {};
	float totalTime = 0.0f;
};

class Light
{
public:
	Light() {};
	virtual void update(LightingManager* manager, float totalTime) = 0;
	virtual void gui() = 0;
	virtual void destroy(GfxContext const& gfx) = 0;

	const char* name = nullptr;
	bool updated = false;
	bool animate = false;
	Animation* animation = nullptr;
	glm::vec3 color = glm::vec3(1.0f);
	glm::vec3 ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
private:
};

class Directional : public Light
{
public:
	virtual void update(LightingManager* manage, float totalTimer);
	virtual void gui();
	virtual void destroy(GfxContext const& gfx);

	DirectionalInput* input = nullptr;
};

class Point : public Light
{
public:
	virtual void update(LightingManager* manager, float totalTime);
	virtual void gui();
	virtual void destroy(GfxContext const& gfx);

	PointInput* input = nullptr;
};

