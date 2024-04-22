#include "Light.h"
#include <glm/ext/matrix_transform.hpp>

void Directional::update(LightingManager* manager, float totalTime)
{
	glm::vec3 diffuseColor = glm::vec3(color) * diffuse;
	glm::vec3 ambientColor = diffuseColor * ambient;

	input->ambient = glm::vec4(ambientColor, 0.0f);
	input->diffuse = glm::vec4(diffuseColor, 0.0f);
	input->specular = glm::vec4(specular, 1.0f);

	manager->updateDirectionals = true;
}

void Directional::gui()
{
	updated |= ImGui::InputFloat3("Direction", &input->direction[0]);
	ImGui::Separator();
	updated |= ImGui::InputFloat3("Ambient", &ambient[0]);
	updated |= ImGui::InputFloat3("Diffuse", &diffuse[0]);
	updated |= ImGui::InputFloat3("Specular", &specular[0]);
    ImGui::Separator();
	updated |= ImGui::ColorPicker3("Color", &color[0]);
    ImGui::Separator();
}

void Directional::destroy(GfxContext const& gfx)
{
}

void Point::update(LightingManager* manager, float totalTime)
{
    // Update light colors as before
    glm::vec3 diffuseColor = glm::vec3(color) * diffuse;
    glm::vec3 ambientColor = diffuseColor * ambient;
    input->ambient = glm::vec4(ambientColor, 1.0f);
    input->diffuse = glm::vec4(diffuseColor, 1.0f);
    input->specular = glm::vec4(specular, 1.0f);

	animation->run(input->position, totalTime);

    // Modulate attenuation factors
    float cycle = (sin(totalTime * 0.5f) + 1.0f) * 0.5f;  // Normalized to range [0, 1]
    input->pLinear = 0.00f + 0.02f * cycle;  // Ranges from 0.045 to 0.065
    input->quadratic = 0.0000f + 0.0025f * cycle;  // Ranges from 0.0075 to 0.01

    manager->updatePoints = true;
}

void Point::gui()
{
	updated |= ImGui::InputFloat3("Position", &input->position[0]);
	ImGui::Separator();
	updated |= ImGui::InputFloat3("Ambient", &ambient[0]);
	updated |= ImGui::InputFloat3("Diffuse", &diffuse[0]);
	updated |= ImGui::InputFloat3("Specular", &specular[0]);
    ImGui::Separator();
	updated |= ImGui::ColorPicker3("Color", &color[0]);
    ImGui::Separator();
}

void Point::destroy(GfxContext const& gfx)
{
}

void LightingManager::initialize(GfxContext const& gfx)
{
}

void LightingManager::update(GfxContext const& gfx, GfxProgram const& program, double deltaTime)
{
	totalTime += deltaTime;
	for (Light* light : lights)
	{
		if (light->updated || light->animate)
		{
			light->update(this, totalTime);
			light->updated = false;
		}
	}

	if (updateDirectionals)
	{
		if (!directionalLights.empty())
		{
			std::vector<DirectionalInput> tempInputs;
			for (DirectionalInput* input : directionalLights)
			{
				tempInputs.push_back(*input);
			}
			gfxUpdateDynamicBuffer(gfx, directionalLightsBuffer, tempInputs.size() * sizeof(DirectionalInput), tempInputs.data());
			gfxProgramSetParameter(gfx, program, "dirLights", directionalLightsBuffer);
		}
		updateDirectionals = false;
	}

	if (updatePoints)
	{
		if (!pointLights.empty())
		{
			std::vector<PointInput> tempInputs;
			for (PointInput* input : pointLights)
			{
				tempInputs.push_back(*input);
			}
			gfxUpdateDynamicBuffer(gfx, pointLightsBuffer, tempInputs.size() * sizeof(PointInput), tempInputs.data());
			gfxProgramSetParameter(gfx, program, "pointLights", pointLightsBuffer);
			gfxProgramSetParameter(gfx, program, "numPointLights", pointLights.size());
		}
		updatePoints = false;
	}
}

void LightingManager::addDirectionalLight(GfxContext const& gfx)
{
	DirectionalInput* input = new DirectionalInput();
	directionalLights.push_back(input);

	Directional* newLight = new Directional();
	char* name = nullptr;
	std::string newName = std::string("Directional light");
	name = new char[strlen(newName.c_str()) + 1];
	strcpy_s(name, strlen(newName.c_str()) + 1, newName.c_str());
	newLight->name = name;
	newLight->updated = true;
	newLight->input = input;
	lights.push_back(newLight);

	gfxDestroyBuffer(gfx, directionalLightsBuffer);
	std::vector<DirectionalInput> tempInputs;
	for (DirectionalInput* input : directionalLights)
		tempInputs.push_back(*input);
	directionalLightsBuffer = gfxCreateBuffer<DirectionalInput>(gfx, tempInputs.size(), tempInputs.data(), kGfxCpuAccess_Write);
}

Light* LightingManager::addPointLight(GfxContext const& gfx, glm::vec3 const* position, glm::vec3 const* color)
{
	PointInput* input = new PointInput();
	if (position)
		input->position = *position;
	pointLights.push_back(input);

	Point* newLight = new Point();
	char* name = nullptr;
	std::string newName = std::string("Point light") + std::to_string(pointLights.size() + 1);
	name = new char[strlen(newName.c_str()) + 1];
	strcpy_s(name, strlen(newName.c_str()) + 1, newName.c_str());
	newLight->name = name;
	newLight->updated = true;
	newLight->input = input;
	if (color)
		newLight->color = *color;

	lights.push_back(newLight);

	gfxDestroyBuffer(gfx, pointLightsBuffer);
	std::vector<PointInput> tempInputs;
	for (PointInput* input : pointLights)
		tempInputs.push_back(*input);
	pointLightsBuffer = gfxCreateBuffer<PointInput>(gfx, tempInputs.size(), tempInputs.data(), kGfxCpuAccess_Write);

	return newLight;
}

void LightingManager::destroy(GfxContext const& gfx)
{
	gfxDestroyBuffer(gfx, directionalLightsBuffer);
	gfxDestroyBuffer(gfx, pointLightsBuffer);

	for (PointInput* input : pointLights)
	{
		if(input != nullptr)
			delete input;

	}
	for (DirectionalInput* input : directionalLights)
	{
		if(input != nullptr)
			delete input;
	}

	for (Light* light : lights)
	{
		delete light->name;
		if (light->animation != nullptr)
			delete light->animation;
		delete light;
	}
}

std::vector<Light*> const* LightingManager::getLights()
{
	return &lights;
}
