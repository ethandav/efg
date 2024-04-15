#include "GameObjects.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

void Mesh::draw(GfxContext const& gfx, GfxProgram const& program)
{
    gfxProgramSetParameter(gfx, program, "transform", modelMatrix);
	gfxProgramSetParameter(gfx, program, "MaterialBuffer", materialBuffer);

    if (hasTexture)
    {
        gfxProgramSetParameter(gfx, program, "AlbedoBuffer", texture);
        gfxProgramSetParameter(gfx, program, "useTexture", true);
    }
	else
	{
		gfxProgramSetParameter(gfx, program, "useTexture", false);
	}

    gfxCommandBindIndexBuffer(gfx, indexBuffer);
    gfxCommandBindVertexBuffer(gfx, vertexBuffer);
    gfxCommandDrawIndexed(gfx, (uint32_t)indices.size());

}

void Mesh::gui()
{
	ImGui::InputFloat3("Position", &position[0], "%.3f");
	ImGui::InputFloat3("Rotation", &rotation[0], "%.3f");
	ImGui::InputFloat3("Scale", &scale[0], "%.3f");
}

void Instanced::draw(GfxContext const& gfx, GfxProgram const& program)
{
	gfxProgramSetParameter(gfx, program, "MaterialBuffer", materialBuffer);
}

void Instanced::gui()
{
	ImGui::InputFloat3("Position", &position[0], "%.3f");
	ImGui::InputFloat3("Rotation", &rotation[0], "%.3f");
	ImGui::InputFloat3("Scale", &scale[0], "%.3f");
}

void LightObject::draw(GfxContext const& gfx, GfxProgram const& program)
{
	glm::vec3 diffuseColor = glm::vec3(color) * diffuse;
	glm::vec3 ambientColor = diffuseColor * ambient;

	lightBuffer.light.ambientColor = glm::vec4(ambientColor, 0.0f);
	lightBuffer.light.diffuseColor = glm::vec4(diffuseColor, 0.0f);
	lightBuffer.light.position = glm::vec4(position, 1.0f);
	lightBuffer.light.specular = glm::vec4(specular, 1.0f);
    gfxProgramSetParameter(gfx, program, "LightBuffer", lightBuffer);
}

void LightObject::gui()
{
	ImGui::InputFloat3("Position", &position[0], "%.3f");
	ImGui::InputFloat3("Rotation", &rotation[0], "%.3f");
	ImGui::InputFloat3("Scale", &scale[0], "%.3f");
    ImGui::Separator();
    ImGui::SliderFloat3("Ambient", &ambient[0], 0.0f, 1.0f, "%.3f");
    ImGui::Separator();
    ImGui::SliderFloat3("Diffuse", &diffuse[0], 0.0f, 1.0f);
    ImGui::Separator();
    ImGui::SliderFloat("Specular", &specular[0], 0.0f, 1.0f);
    ImGui::Separator();
	ImGui::ColorPicker3("Color", &color[0]);
    ImGui::Separator();
}

glm::mat4 CreateModelMatrix(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale)
{
	glm::vec3 rotationRadians = glm::radians(rotation);
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), translation);
	glm::mat4 rotationMatrix = glm::yawPitchRoll(rotationRadians.y, rotationRadians.x, rotationRadians.z);
	glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), scale);
	return translationMatrix * rotationMatrix * scalingMatrix;
}
