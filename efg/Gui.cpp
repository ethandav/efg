#include "Gui.h"

void Gui::initialize(const GfxContext& gfx)
{
	gfxImGuiInitialize(gfx);
}

void Gui::update(GfxContext const& gfx, GfxWindow const& window)
{
	ImGui::Begin("Ethan's Framework (for) Graphics", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings);
	ImGui::End();
	gfxImGuiRender();
}

void Gui::destroy(GfxContext const& gfx)
{
	gfxImGuiTerminate();
}
