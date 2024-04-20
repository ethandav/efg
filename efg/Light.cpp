#include "Light.h"

void Directional::update(LightingManager* manager)
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

void Point::update(LightingManager* manager)
{
	glm::vec3 diffuseColor = glm::vec3(color) * diffuse;
	glm::vec3 ambientColor = diffuseColor * ambient;

	input->ambient = glm::vec4(ambientColor, 0.0f);
	input->diffuse = glm::vec4(diffuseColor, 0.0f);
	input->specular = glm::vec4(specular, 1.0f);

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

void LightingManager::update(GfxContext const& gfx, GfxProgram const& program)
{
	for (Light* light : lights)
	{
		if (light->updated)
		{
			light->update(this);
			light->updated = false;
		}
	}

	if (updateDirectionals)
	{
		gfxDestroyBuffer(gfx, directionalLightsBuffer);
		if (!directionalLights.empty())
		{
			std::vector<DirectionalInput> tempInputs;
			for (DirectionalInput* input : directionalLights)
			{
				tempInputs.push_back(*input);
			}
			directionalLightsBuffer = gfxCreateBuffer<DirectionalInput>(gfx, tempInputs.size(), tempInputs.data());
			gfxProgramSetParameter(gfx, program, "dirLights", directionalLightsBuffer);
		}
		updateDirectionals = false;
	}

	if (updatePoints)
	{
		gfxDestroyBuffer(gfx, pointLightsBuffer);
		if (!pointLights.empty())
		{
			std::vector<PointInput> tempInputs;
			for (PointInput* input : pointLights)
			{
				tempInputs.push_back(*input);
			}
			pointLightsBuffer = gfxCreateBuffer<PointInput>(gfx, tempInputs.size(), tempInputs.data());
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
}

void LightingManager::addPointLight(GfxContext const& gfx)
{
	PointInput* input = new PointInput();
	pointLights.push_back(input);

	Point* newLight = new Point();
	char* name = nullptr;
	std::string newName = std::string("Point light") + std::to_string(pointLights.size() + 1);
	name = new char[strlen(newName.c_str()) + 1];
	strcpy_s(name, strlen(newName.c_str()) + 1, newName.c_str());
	newLight->name = name;
	newLight->updated = true;
	newLight->input = input;
	lights.push_back(newLight);
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
		delete light;
	}
}

std::vector<Light*> const* LightingManager::getLights()
{
	return &lights;
}
