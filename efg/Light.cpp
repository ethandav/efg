#include "Light.h"

Light::Light(LightType type) : type(type)
{
	if (type == DIRECTIONAL)
	{
		directional = new Directional();
	}
}

Directional* Light::getDirectionalLight()
{
	return directional;
}

void Light::update(GfxContext const& gfx, GfxProgram const& program)
{
	if (type == DIRECTIONAL)
	{
		glm::vec3 diffuseColor = glm::vec3(color) * diffuse;
		glm::vec3 ambientColor = diffuseColor * ambient;

		directional->ambient = glm::vec4(ambientColor, 0.0f);
		directional->diffuse = glm::vec4(diffuseColor, 0.0f);
		directional->specular = glm::vec4(specular, 1.0f);
	}
}

void Light::gui()
{
	if (type == DIRECTIONAL)
	{
		if (ImGui::InputFloat3("Position", &directional->direction[0]))
			updateLight = true;
		ImGui::Separator();
	}
	if(ImGui::InputFloat3("Ambient", &ambient[0]))
			updateLight = true;
	if(ImGui::InputFloat3("Diffuse", &diffuse[0]))
			updateLight = true;
	if(ImGui::InputFloat3("Specular", &specular[0]))
			updateLight = true;
    ImGui::Separator();
	if(ImGui::ColorPicker3("Color", &color[0]))
			updateLight = true;
    ImGui::Separator();
}

void Light::destroy(GfxContext const& gfx)
{
	if (directional != nullptr)
		delete directional;
}
