#include "GameObjects.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

void Mesh::draw(GfxContext const& gfx, GfxProgram const& program)
{
    gfxProgramSetParameter(gfx, program, "transform", modelMatrix);
    if (material.hasTexture)
    {
        gfxProgramSetParameter(gfx, program, "AlbedoBuffer", material.texture);
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
	ImGui::TreePop();
}

void Light::draw(GfxContext const& gfx, GfxProgram const& program)
{
    gfxProgramSetParameter(gfx, program, "lightColor", lightColor);
    gfxProgramSetParameter(gfx, program, "lightPosition", position);
    gfxProgramSetParameter(gfx, program, "lightIntensity", lightIntensity);
    gfxProgramSetParameter(gfx, program, "specStrength", specStrength);
    gfxProgramSetParameter(gfx, program, "shininess", shininess);
}

void Light::gui()
{
	ImGui::InputFloat3("Position", &position[0], "%.3f");
	ImGui::InputFloat3("Rotation", &rotation[0], "%.3f");
	ImGui::InputFloat3("Scale", &scale[0], "%.3f");
    ImGui::Separator();
    ImGui::SliderFloat("Ambient Light Intensity", &lightIntensity, 0, 5);
    ImGui::Separator();
    ImGui::SliderFloat("Specular Strength", &specStrength, 0, 5);
    ImGui::Separator();
    ImGui::InputInt("Shininess", &shininess, 2, 256);
    ImGui::Separator();
    ImGui::ColorPicker3("Light Source Color", lightColor);
    ImGui::Separator();
	ImGui::TreePop();
}

glm::mat4 CreateModelMatrix(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale)
{
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), translation);
	glm::mat4 rotationMatrix = glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);
	glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), scale);
	return translationMatrix * rotationMatrix * scalingMatrix;
}
