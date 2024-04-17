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
				if (ImGui::MenuItem("Plane"))
				{
					scene->AddPrimitive(
						gfx,
						nullptr,
						Shapes::PLANE
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
					scene->addDirectionalLight(gfx);
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

	std::vector<GameObject*>* objects = scene->getGameObjects();
	if (!objects->empty())
	{
		uint32_t i = 0;
		ImGui::Begin("Game Objects", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings);
		for (GameObject* obj : *objects)
		{
			if (obj != nullptr)
			{
				if (ImGui::TreeNode(obj->name))
				{
					obj->gui();

					std::string buttonId = "Delete##" + std::to_string(i);
					if (ImGui::Button(buttonId.c_str()))
					{
						objects->erase(objects->begin() + i);
						obj->destroy(gfx);
						delete obj->name;
						delete obj;
						--i;
					}
					ImGui::TreePop();
				}
				++i;
			}
		}
		ImGui::End();
	}
	gfxImGuiRender();
}

void Gui::destroy(GfxContext const& gfx)
{
	gfxImGuiTerminate();
}
