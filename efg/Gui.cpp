#include "Gui.h"

void Gui::initialize(const GfxContext& gfx)
{
	gfxImGuiInitialize(gfx);
}

void Gui::update(GfxContext const& gfx, GfxWindow const& window)
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Add Objects"))
		{
			if (ImGui::BeginMenu("Shape"))
			{
				if (ImGui::MenuItem("Square"))
				{
					scene->AddPrimitive(
						gfx,
						nullptr,
						Shapes::SQUARE
					);
				}
				if (ImGui::MenuItem("Cube"))
				{
					scene->AddPrimitive(
						gfx,
						nullptr,
						Shapes::CUBE
					);
				}
				if (ImGui::MenuItem("Sphere"))
				{
					scene->AddPrimitive(
						gfx,
						nullptr,
						Shapes::SPHERE
					);
				}
				if (ImGui::MenuItem("Triangle"))
				{
					scene->AddPrimitive(
						gfx,
						nullptr,
						Shapes::TRIANGLE
					);
				}
				if (ImGui::MenuItem("Pyramid"))
				{
					scene->AddPrimitive(
						gfx,
						nullptr,
						Shapes::PYRAMID
					);
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Light"))
			{
				if (ImGui::MenuItem("Directional"))
				{
					scene->addLight(gfx, "Directional Light");
				}
				if (ImGui::MenuItem("Point"))
				{
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	ImGui::Begin("Game Objects", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings);
	for (GameObject* obj : *scene->getGameObjects())
	{
		if (obj != nullptr)
		{
			if (ImGui::TreeNode(obj->name))
			{
				obj->gui();
			}
		}
	}
	ImGui::End();
	gfxImGuiRender();
}

void Gui::destroy(GfxContext const& gfx)
{
	gfxImGuiTerminate();
}
