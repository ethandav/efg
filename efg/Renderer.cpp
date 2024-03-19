#include "Renderer.h"

void Renderer::start(GfxWindow window)
{
	float time = 0.0f;
	m_Window = &window;
	gfx = gfxCreateContext(window);
	gfxScene = gfxCreateScene();

	gfxImGuiInitialize(gfx);

	cam = CreateFlyCamera(gfx, glm::vec3(0.0f, 0.0f, -10.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	//LoadScene("assets/sponza.obj");
	GfxRef<GfxInstance> obj1 = AddPrimitiveToScene(Shapes::SPHERE, "assets/textures/earth.jpeg");
	GfxRef<GfxInstance> obj2 = AddPrimitiveToScene(Shapes::SPHERE, "assets/textures/moon.jpg");
	obj2->transform = CreateTransformationMatrix(
		glm::vec3(1.0, 1.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(.2f, .2f, .2f)
	);

	glm::vec3 lightPosition = glm::vec3(17.0f, 17.0f, -20.0f);
	float lightColor[3] = {1.0f, 1.0f, 1.0f};
	float lightIntensity = 0.0f;
	float specStrength = 0.0f;
	int shininess = 32;

	uint32_t const instanceCount = gfxSceneGetInstanceCount(gfxScene);

	GfxDrawState drawState;
	GfxTexture colorBuffer = gfxCreateTexture2D(gfx, DXGI_FORMAT_R16G16B16A16_FLOAT);
    GfxTexture depthBuffer = gfxCreateTexture2D(gfx, DXGI_FORMAT_D32_FLOAT);
	gfxDrawStateSetColorTarget(drawState, 0, colorBuffer.getFormat());
	gfxDrawStateSetDepthStencilTarget(drawState, depthBuffer.getFormat());
	gfxDrawStateSetDepthFunction(drawState, D3D12_COMPARISON_FUNC_LESS);

	GfxProgram litProgram = gfxCreateProgram(gfx, "lit");
	GfxKernel litKernel = gfxCreateGraphicsKernel(gfx, litProgram, drawState);
	GfxKernel resolveKernel = gfxCreateGraphicsKernel(gfx, litProgram, "resolve");
	GfxSamplerState textureSampler = gfxCreateSamplerState(gfx, D3D12_FILTER_ANISOTROPIC, D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_WRAP);

	gfxProgramSetParameter(gfx, litProgram, "TextureSampler", textureSampler);
	gfxProgramSetParameter(gfx, litProgram, "ColorBuffer", colorBuffer);

	while (!gfxWindowIsCloseRequested(window))
	{

	    //float angularSpeed = glm::pi<float>() / 20.0;
		//float angle = angularSpeed * time;
		//
		//lightPosition.x = 1.5 * glm::cos(angle);
		//lightPosition.z = 1.5 * glm::sin(angle);
		//lightPosition.y = 1.5 * glm::cos((glm::pi<float>() / 30.0) * time);

		UpdateFlyCamera(gfx, *m_Window, cam);
		gfxProgramSetParameter(gfx, litProgram, "g_ViewProjection", cam.view_proj);

		gfxWindowPumpEvents(window);

		gfxCommandBindColorTarget(gfx, 0, colorBuffer);
		gfxCommandBindDepthStencilTarget(gfx, depthBuffer);
		gfxCommandBindKernel(gfx, litKernel);
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

		for (uint32_t i = 0; i < instanceCount; ++i)
		{
			GfxInstance const& instance = gfxSceneGetInstances(gfxScene)[i];

			gfxProgramSetParameter(gfx, litProgram, "transform", instance.transform);

			if (instance.material)
			{
				gfxProgramSetParameter(gfx, litProgram, "AlbedoBuffer", albedoBuffers[instance.material]);
			}
			else
			{
				gfxProgramSetParameter(gfx, litProgram, "AlbedoBuffer", GfxTexture());
			}
			gfxCommandBindIndexBuffer(gfx, indexBuffers[instance.mesh]);
			gfxCommandBindVertexBuffer(gfx, vertexBuffers[instance.mesh]);
			gfxCommandDrawIndexed(gfx, (uint32_t)instance.mesh->indices.size());
		}

		gfxCommandBindKernel(gfx, resolveKernel);
		gfxCommandDraw(gfx, 3);

		gfxImGuiRender();
		gfxFrame(gfx);

		time += 0.1f;
	}


	gfxDestroyTexture(gfx, depthBuffer);
	gfxDestroyTexture(gfx, colorBuffer);
	gfxDestroyKernel(gfx, litKernel);
	gfxDestroyKernel(gfx, resolveKernel);
	gfxDestroyProgram(gfx, litProgram);
	gfxDestroySamplerState(gfx, textureSampler);

    for(uint32_t i = 0; i < indexBuffers.size(); ++i)
        gfxDestroyBuffer(gfx, indexBuffers.data()[i]);
    for(uint32_t i = 0; i < vertexBuffers.size(); ++i)
        gfxDestroyBuffer(gfx, vertexBuffers.data()[i]);
    for(uint32_t i = 0; i < albedoBuffers.size(); ++i)
        gfxDestroyTexture(gfx, albedoBuffers.data()[i]);

	gfxDestroyWindow(window);
	shutdown();
}

GfxRef<GfxInstance> Renderer::AddPrimitiveToScene(const Shapes::Types type, const char* textureFile)
{
	using namespace Shapes;

	Shape shape = {};
	switch (type)
	{
	case CUBE:
		shape = cube();
		break;
	case SPHERE:
		shape = sphere();
		break;
	}

	glm::vec3 translation = glm::vec3(0.0, 0.0f, 0.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::mat4 transformationMatrix = CreateTransformationMatrix(translation, rotation, scale);

	GfxRef<GfxMesh> newMesh = gfxSceneCreateMesh(gfxScene);
	GfxRef<GfxInstance> newInstance = gfxSceneCreateInstance(gfxScene);
	newMesh->indices = shape.indices;
	newMesh->vertices = shape.vertices;
	GfxBuffer indexBuffer = gfxCreateBuffer<uint32_t>(gfx, shape.indexCount, shape.indices.data());
	GfxBuffer vertexBuffer = gfxCreateBuffer<Vertex>(gfx, shape.vertexCount, shape.vertices.data());
	newInstance->mesh = newMesh;
	indexBuffers.insert(newMesh, indexBuffer);
	vertexBuffers.insert(newMesh, vertexBuffer);

	GfxRef<GfxMaterial> matRef = gfxSceneCreateMaterial(gfxScene);
	GfxTexture albedoBuffer;
	gfxSceneImport(gfxScene, textureFile);
	GfxConstRef<GfxImage> imgRef = gfxSceneGetImageHandle(gfxScene, gfxSceneGetImageCount(gfxScene) - 1);
	uint32_t const mipCount = gfxCalculateMipCount(imgRef->width, imgRef->height);
	albedoBuffer = gfxCreateTexture2D(gfx, imgRef->width, imgRef->height, imgRef->format, mipCount);
	GfxBuffer uploadBuffer = gfxCreateBuffer(gfx, imgRef->width * imgRef->height * imgRef->channel_count, imgRef->data.data());
    gfxCommandCopyBufferToTexture(gfx, albedoBuffer, uploadBuffer);
	gfxCommandGenerateMips(gfx, albedoBuffer);
    gfxDestroyBuffer(gfx, uploadBuffer);
	matRef->albedo_map = imgRef;
	albedoBuffers.insert(matRef, albedoBuffer);
	newInstance->material = matRef;

	newInstance->transform = transformationMatrix;

	return newInstance;
}

glm::mat4 Renderer::CreateTransformationMatrix(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale)
{
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), translation);
	glm::mat4 rotationMatrix = glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);
	glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), scale);
	return translationMatrix * rotationMatrix * scalingMatrix;
}

void Renderer::LoadScene(const char* assetFile)
{
    gfxSceneImport(gfxScene, assetFile);   // import our scene

    uint32_t const mesh_count     = gfxSceneGetMeshCount(gfxScene);
    uint32_t const material_count = gfxSceneGetMaterialCount(gfxScene);
    uint32_t const instance_count = gfxSceneGetInstanceCount(gfxScene);

    for(uint32_t i = 0; i < mesh_count; ++i)
    {
        GfxConstRef<GfxMesh> mesh_ref = gfxSceneGetMeshHandle(gfxScene, i);

        uint32_t const index_count  = (uint32_t)mesh_ref->indices.size();
        uint32_t const vertex_count = (uint32_t)mesh_ref->vertices.size();

        std::vector<Vertex> vertices(vertex_count);

        for(uint32_t v = 0; v < vertex_count; ++v)
        {
            vertices[v].position = glm::vec4(mesh_ref->vertices[v].position, 1.0f);
            vertices[v].normal   = glm::vec4(mesh_ref->vertices[v].normal,   0.0f);
            vertices[v].uv       = glm::vec2(mesh_ref->vertices[v].uv);
        }

        GfxBuffer index_buffer  = gfxCreateBuffer<uint32_t>(gfx, index_count, mesh_ref->indices.data());
        GfxBuffer vertex_buffer = gfxCreateBuffer<Vertex>(gfx, vertex_count, vertices.data());

        indexBuffers.insert(mesh_ref, index_buffer);
        vertexBuffers.insert(mesh_ref, vertex_buffer);
    }

    for(uint32_t i = 0; i < material_count; ++i)
    {
        GfxConstRef<GfxMaterial> material_ref = gfxSceneGetMaterialHandle(gfxScene, i);

        GfxTexture albedo_buffer;

        if(material_ref->albedo_map)
        {
            GfxImage const &albedo_map = *material_ref->albedo_map;
            uint32_t const mip_count   = gfxCalculateMipCount(albedo_map.width, albedo_map.height);

            albedo_buffer = gfxCreateTexture2D(gfx, albedo_map.width, albedo_map.height, albedo_map.format, mip_count);

            GfxBuffer upload_buffer = gfxCreateBuffer(gfx, albedo_map.width * albedo_map.height * albedo_map.channel_count, albedo_map.data.data());
            gfxCommandCopyBufferToTexture(gfx, albedo_buffer, upload_buffer);
            gfxCommandGenerateMips(gfx, albedo_buffer);
            gfxDestroyBuffer(gfx, upload_buffer);
        }

        albedoBuffers.insert(material_ref, albedo_buffer);
    }
}

void Renderer::shutdown()
{
	gfxImGuiTerminate();
	gfxDestroyScene(gfxScene);
	gfxDestroyContext(gfx);
}
