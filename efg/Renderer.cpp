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

	gfxCommandBindColorTarget(gfx, 0, colorBuffer);
	gfxCommandBindDepthStencilTarget(gfx, depthBuffer);
	gfxCommandClearTexture(gfx, colorBuffer);
	gfxCommandClearTexture(gfx, depthBuffer);

	ImGui::Begin("Ethan's Framework (for) Graphics", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings);

	gfxProgramSetParameter(gfx, litProgram, "viewPos", cam.eye);

	gfxCommandBindKernel(gfx, litKernel);

	m_scene->update(litProgram);

	gfxCommandBindKernel(gfx, resolveKernel);
	gfxCommandDraw(gfx, 3);

	ImGui::End();
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
