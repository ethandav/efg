#include "Light.h"

void Directional::update(GfxContext const& gfx, GfxProgram const& program)
{
	glm::vec3 diffuseColor = glm::vec3(color) * diffuse;
	glm::vec3 ambientColor = diffuseColor * ambient;

	input.ambient = glm::vec4(ambientColor, 0.0f);
	input.diffuse = glm::vec4(diffuseColor, 0.0f);
	input.specular = glm::vec4(specular, 1.0f);
	gfxDestroyBuffer(gfx, buffer);
	buffer = gfxCreateBuffer<DirectionalInput>(gfx, 1, &input);
	gfxProgramSetParameter(gfx, program, "dirLight", buffer);
}

void Directional::gui()
{
	ImGui::InputFloat3("Position", &input.direction[0]);
	ImGui::Separator();
	ImGui::InputFloat3("Ambient", &ambient[0]);
	ImGui::InputFloat3("Diffuse", &diffuse[0]);
	ImGui::InputFloat3("Specular", &specular[0]);
    ImGui::Separator();
	ImGui::ColorPicker3("Color", &color[0]);
    ImGui::Separator();
}

void Directional::destroy(GfxContext const& gfx)
{
}

void Point::update(GfxContext const& gfx, GfxProgram const& program)
{
	glm::vec3 diffuseColor = glm::vec3(color) * diffuse;
	glm::vec3 ambientColor = diffuseColor * ambient;

	input.ambient = glm::vec4(ambientColor, 0.0f);
	input.diffuse = glm::vec4(diffuseColor, 0.0f);
	input.specular = glm::vec4(specular, 1.0f);
	gfxDestroyBuffer(gfx, *buffer);
	*buffer = gfxCreateBuffer<DirectionalInput>(gfx, 1, &input);
	gfxProgramSetParameter(gfx, program, "dirLight", *buffer);
}

void Point::gui()
{
	ImGui::InputFloat3("Position", &input.direction[0]);
	ImGui::Separator();
	ImGui::InputFloat3("Ambient", &ambient[0]);
	ImGui::InputFloat3("Diffuse", &diffuse[0]);
	ImGui::InputFloat3("Specular", &specular[0]);
    ImGui::Separator();
	ImGui::ColorPicker3("Color", &color[0]);
    ImGui::Separator();
}

void Point::destroy(GfxContext const& gfx)
{
}
