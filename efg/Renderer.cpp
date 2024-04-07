#include "Renderer.h"

void Renderer::start(const GfxContext& gfx, const GfxWindow& window, const Scene& scene)
{
	float time = 0.0f;
	m_Window = &window;
	m_scene = &scene;
	m_gfx = &gfx;

	gfxImGuiInitialize(gfx);

	cam = CreateFlyCamera(gfx, glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	colorBuffer = gfxCreateTexture2D(gfx, DXGI_FORMAT_R16G16B16A16_FLOAT);
    depthBuffer = gfxCreateTexture2D(gfx, DXGI_FORMAT_D32_FLOAT);

	gfxDrawStateSetColorTarget(drawState, 0, colorBuffer.getFormat());
	gfxDrawStateSetDepthStencilTarget(drawState, depthBuffer.getFormat());
	gfxDrawStateSetDepthFunction(drawState, D3D12_COMPARISON_FUNC_LESS);

	litProgram = gfxCreateProgram(gfx, "lit");
	litKernel = gfxCreateGraphicsKernel(gfx, litProgram, drawState);
	resolveKernel = gfxCreateGraphicsKernel(gfx, litProgram, "resolve");
	textureSampler = gfxCreateSamplerState(gfx, D3D12_FILTER_ANISOTROPIC, D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_WRAP);

	gfxProgramSetParameter(gfx, litProgram, "TextureSampler", textureSampler);
	gfxProgramSetParameter(gfx, litProgram, "ColorBuffer", colorBuffer);
}

void Renderer::update()
{
	const GfxContext gfx = *m_gfx;


	UpdateFlyCamera(gfx, *m_Window, cam);
	gfxProgramSetParameter(gfx, litProgram, "g_ViewProjection", cam.view_proj);

	gfxWindowPumpEvents(*m_Window);

	gfxCommandBindColorTarget(gfx, 0, colorBuffer);
	gfxCommandBindDepthStencilTarget(gfx, depthBuffer);
	gfxCommandClearTexture(gfx, colorBuffer);
	gfxCommandClearTexture(gfx, depthBuffer);

	if (ImGui::Begin("Lighting", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings))
	{
	    ImGui::Separator();
	    if (ImGui::SliderFloat("Camera Distance", &cam.eye.z, 0.0f, 100.0f))
	    {
	        updateView(gfx, cam);
	    }
	    ImGui::Separator();
	    ImGui::SliderFloat("Ambient Light Intensity", &lightIntensity, 0, 5);
	    ImGui::Separator();
	    ImGui::SliderFloat("Specular Strength", &specStrength, 0, 5);
	    ImGui::Separator();
	    ImGui::InputInt("Shininess", &shininess, 2, 256);
	    ImGui::Separator();
	    ImGui::Text("Light Position");
	    ImGui::SliderFloat("x", &lightPosition.x, -20, 20);
	    ImGui::SliderFloat("y", &lightPosition.y, -20, 20);
	    ImGui::SliderFloat("z", &lightPosition.z, -20, 20);
	    ImGui::Separator();
	    ImGui::ColorPicker3("Light Source Color", lightColor);
	    ImGui::Separator();
	}
	ImGui::End();

	gfxProgramSetParameter(gfx, litProgram, "lightColor", lightColor);
	gfxProgramSetParameter(gfx, litProgram, "lightPosition", lightPosition);
	gfxProgramSetParameter(gfx, litProgram, "lightIntensity", lightIntensity);
	gfxProgramSetParameter(gfx, litProgram, "specStrength", specStrength);
	gfxProgramSetParameter(gfx, litProgram, "shininess", shininess);
	gfxProgramSetParameter(gfx, litProgram, "viewPos", cam.eye);

	gfxCommandBindKernel(gfx, litKernel);

	m_scene->update(litProgram);

	gfxCommandBindKernel(gfx, resolveKernel);
	gfxCommandDraw(gfx, 3);

	gfxImGuiRender();
	gfxFrame(gfx);
}

void Renderer::shutdown()
{
	const GfxContext gfx = *m_gfx;

	gfxDestroyTexture(gfx, depthBuffer);
	gfxDestroyTexture(gfx, colorBuffer);
	gfxDestroyKernel(gfx, litKernel);
	gfxDestroyKernel(gfx, resolveKernel);
	gfxDestroyProgram(gfx, litProgram);
	gfxDestroySamplerState(gfx, textureSampler);

	gfxImGuiTerminate();
	gfxDestroyContext(gfx);
}
