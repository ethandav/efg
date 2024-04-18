#pragma once
#include "RenderLayer.h"

struct Directional
{
	glm::vec3 direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	glm::vec3 ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
};

class Light
{
public:
	enum LightType { DIRECTIONAL, POINT};
	Light(LightType type);
	Directional* getDirectionalLight();
	void update(GfxContext const& gfx, GfxProgram const& program);
	void gui();
	void destroy(GfxContext const& gfx);

	bool updateLight = true;
	const char* name = nullptr;

private:
	LightType type;
	Directional* directional = nullptr;
	glm::vec3 color = glm::vec3(1.0f);
	glm::vec3 ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
};
