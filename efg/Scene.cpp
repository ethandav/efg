#include "Scene.h"

void Scene::initialize(const GfxContext& gfx)
{
	gfxScene = gfxCreateScene();

	cam = CreateFlyCamera(gfx, glm::vec3(0.0f, 5.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	colorBuffer = gfxCreateTexture2D(gfx, DXGI_FORMAT_R16G16B16A16_FLOAT);
    depthBuffer = gfxCreateTexture2D(gfx, DXGI_FORMAT_D32_FLOAT);

	gfxDrawStateSetColorTarget(drawState, 0, colorBuffer.getFormat());
	gfxDrawStateSetDepthStencilTarget(drawState, depthBuffer.getFormat());
	gfxDrawStateSetDepthFunction(drawState, D3D12_COMPARISON_FUNC_LESS);

	litProgram = gfxCreateProgram(gfx, "lit");
	litKernel = gfxCreateGraphicsKernel(gfx, litProgram, drawState);
	litResolveKernel = gfxCreateGraphicsKernel(gfx, litProgram, "resolve");

	skyboxProgram = gfxCreateProgram(gfx, "sky");
	skyboxKernel = gfxCreateGraphicsKernel(gfx, skyboxProgram, drawState);
	skyboxResolveKernel = gfxCreateGraphicsKernel(gfx, skyboxProgram, "resolve");

	textureSampler = gfxCreateSamplerState(gfx, D3D12_FILTER_ANISOTROPIC, D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_WRAP);

	gfxProgramSetParameter(gfx, litProgram, "TextureSampler", textureSampler);
	gfxProgramSetParameter(gfx, litProgram, "ColorBuffer", colorBuffer);

	gfxProgramSetParameter(gfx, skyboxProgram, "TextureSampler", textureSampler);
	gfxProgramSetParameter(gfx, skyboxProgram, "ColorBuffer", colorBuffer);

    loadScene(gfx);
}

void Scene::loadScene(const GfxContext& gfx)
{
	createSkybox(gfx, "assets/textures/sky.jpg");

	addLight(gfx, "Light 1",
		glm::vec3(0.0, 2.0f, 0.0f)
	);

	//AddPrimitive(
	//	gfx,
	//	"Sphere 1",
	//	Shapes::SPHERE,
	//	nullptr,
	//	glm::vec3(-2.0, 0.0f, 2.0f)
	//);

	//AddPrimitive(
	//	gfx,
	//	"Sphere 2",
	//	Shapes::SPHERE,
	//	nullptr,
	//	glm::vec3(2.0, 0.0f, 2.0f)
	//);

	//AddPrimitive(
	//	gfx,
	//	"Sphere 3",
	//	Shapes::SPHERE,
	//	nullptr,
	//	glm::vec3(0.0, 0.0f, -2.0f)
	//);

	//AddPrimitive(gfx, "Earth", Shapes::SPHERE, "assets/textures/earth.jpeg");

	//AddPrimitive(
    //    gfx,
	//	"Moon",
	//	Shapes::SPHERE,
	//	"assets/textures/moon.jpg",
	//	glm::vec3(1.0, 1.0f, -15.0f),
	//	glm::vec3(0.0f, 0.0f, 0.0f),
	//	glm::vec3(.5f, .5f, .5f)
	//);

	//AddPrimitive(
    //    gfx,
	//	"Sun",
	//	Shapes::SPHERE,
	//	"assets/textures/sun.png",
	//	glm::vec3(0.0, 0.0f, -80.0f),
	//	glm::vec3(0.0f, 0.0f, 0.0f),
	//	glm::vec3(20.0f, 20.0f, 20.0f)
	//);

	LoadSceneFromFile(gfx, "assets/sponza.obj");
}

void Scene::update(GfxContext const& gfx, GfxWindow const& window)
{
	UpdateFlyCamera(gfx, window, cam);

	gfxCommandBindColorTarget(gfx, 0, colorBuffer);
	gfxCommandBindDepthStencilTarget(gfx, depthBuffer);
	gfxCommandClearTexture(gfx, colorBuffer);
	gfxCommandClearTexture(gfx, depthBuffer);
	gfxProgramSetParameter(gfx, litProgram, "g_ViewProjection", cam.view_proj);
	gfxProgramSetParameter(gfx, litProgram, "viewPos", cam.eye);
	gfxProgramSetParameter(gfx, skyboxProgram, "g_View", glm::mat4(glm::mat3(cam.view)));
	gfxProgramSetParameter(gfx, skyboxProgram, "g_Projection", cam.proj);

	updateSkybox(gfx);
    updateGameObjects(gfx);

	gfxCommandBindKernel(gfx, skyboxResolveKernel);
	gfxCommandDraw(gfx, 3);

	gfxCommandBindKernel(gfx, litResolveKernel);
	gfxCommandDraw(gfx, 3);

}

void Scene::updateGameObjects(GfxContext const& gfx)
{
	gfxCommandBindKernel(gfx, litKernel);

    for (GameObject* obj : gameObjects)
    {
        if (obj == nullptr)
            continue;

		bool positionChanged = obj->position != obj->prevPosition;
		bool rotationChanged = obj->rotation != obj->prevRotation;
		bool scaleChanged = obj->scale != obj->prevScale;

		if (positionChanged || rotationChanged || scaleChanged)
		{
		    obj->prevPosition = obj->position;
		    obj->prevRotation = obj->rotation;
		    obj->prevScale = obj->scale;
		    obj->modelMatrix = CreateModelMatrix(obj->position, obj->rotation, obj->scale);
		}

		if (obj->reference)
		{
			DrawInstanced(gfx, obj);
		}
		obj->draw(gfx, litProgram);

		if (ImGui::TreeNode(obj->name))
		{
			obj->gui();
		}
    }
}

void Scene::DrawInstanced(GfxContext const& gfx, GameObject* obj)
{
	GfxInstance* instance = gfxSceneGetInstance(gfxScene, obj->reference);

	gfxProgramSetParameter(gfx, litProgram, "transform", instance->transform);
	
	if (instance->material)
	{
	    gfxProgramSetParameter(gfx, litProgram, "AlbedoBuffer", albedoBuffers[instance->material]);
	    gfxProgramSetParameter(gfx, litProgram, "useTexture", true);
	}
	else
	{
	    gfxProgramSetParameter(gfx, litProgram, "AlbedoBuffer", GfxTexture());
	}
	
	gfxCommandBindIndexBuffer(gfx, indexBuffers[instance->mesh]);
	gfxCommandBindVertexBuffer(gfx, vertexBuffers[instance->mesh]);
	gfxCommandDrawIndexed(gfx, (uint32_t)instance->mesh->indices.size());
}

void Scene::updateSkybox(GfxContext const& gfx)
{
	gfxCommandBindKernel(gfx, skyboxKernel);
    gfxProgramSetParameter(gfx, skyboxProgram, "transform", glm::mat4(1.0f));
	gfxProgramSetParameter(gfx, skyboxProgram, "AlbedoBuffer", skybox->material.texture);
	gfxCommandBindIndexBuffer(gfx, skybox->indexBuffer);
	gfxCommandBindVertexBuffer(gfx, skybox->vertexBuffer);
	gfxCommandDrawIndexed(gfx, (uint32_t)skybox->indices.size());
}

void Scene::addLight(GfxContext const& gfx, const char* name, glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale)
{
	char* lightName = new char[strlen(name) + 1];
	strcpy_s(lightName, strlen(name) + 1, name);
	Light* light = new Light(lightName, translation, rotation, scale);
	gameObjects.push_back(light);
}

void Scene::AddPrimitive(GfxContext const& gfx, const char* name, const Shapes::Types type, const char* textureFile,
	glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale)
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
	case TRIANGLE:
		shape = triangle();
		break;
	case PYRAMID:
		shape = pyramid();
		break;
	}

	Mesh* newMesh = new Mesh();
	newMesh->position = translation;
	newMesh->rotation = rotation;
	newMesh->scale = scale;

	glm::mat4 modelMatrix = CreateModelMatrix(translation, rotation, scale);

	newMesh->indices = shape.indices;
	newMesh->vertices = shape.vertices;
	newMesh->indexBuffer = gfxCreateBuffer<uint32_t>(gfx, shape.indexCount, shape.indices.data());
	newMesh->vertexBuffer = gfxCreateBuffer<Vertex>(gfx, shape.vertexCount, shape.vertices.data());

	if (textureFile != nullptr)
	{
		gfxSceneImport(gfxScene, textureFile);
		GfxConstRef<GfxImage> imgRef = gfxSceneGetImageHandle(gfxScene, gfxSceneGetImageCount(gfxScene) - 1);
		uint32_t const mipCount = gfxCalculateMipCount(imgRef->width, imgRef->height);
		newMesh->material.texture = gfxCreateTexture2D(gfx, imgRef->width, imgRef->height, imgRef->format, mipCount);
		GfxBuffer uploadBuffer = gfxCreateBuffer(gfx, imgRef->width * imgRef->height * imgRef->channel_count, imgRef->data.data());
		gfxCommandCopyBufferToTexture(gfx, newMesh->material.texture, uploadBuffer);
		gfxCommandGenerateMips(gfx, newMesh->material.texture);
		gfxDestroyBuffer(gfx, uploadBuffer);
		newMesh->material.hasTexture = true;
	}

	newMesh->modelMatrix = modelMatrix;

	char* objName = new char[strlen(name) + 1];
	strcpy_s(objName, strlen(name) + 1, name);
	newMesh->name = objName;
	gameObjects.push_back(newMesh);
}


void Scene::LoadSceneFromFile(GfxContext const& gfx, const char* assetFile)
{
    uint32_t const orig_instance_count = gfxSceneGetInstanceCount(gfxScene);
    uint32_t const orig_mesh_count     = gfxSceneGetMeshCount(gfxScene);
    uint32_t const orig_material_count = gfxSceneGetMaterialCount(gfxScene);

	gfxSceneImport(gfxScene, assetFile);

    uint32_t const new_instance_count = gfxSceneGetInstanceCount(gfxScene);
    uint32_t const new_mesh_count     = gfxSceneGetMeshCount(gfxScene);
    uint32_t const new_material_count = gfxSceneGetMaterialCount(gfxScene);


    for(uint32_t i = orig_mesh_count; i < new_mesh_count; ++i)
    {
        GfxConstRef<GfxMesh> mesh_ref = gfxSceneGetMeshHandle(gfxScene, i);
		GfxRef<GfxInstance> inst_ref = gfxSceneGetInstanceHandle(gfxScene, i);

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

		std::string tmp_name = std::string("Imported Mesh ") + std::to_string(i);
		char* name = new char[tmp_name.length() + 1];
		strcpy_s(name, tmp_name.length() + 1, tmp_name.c_str());

		gameObjects.push_back(new Instanced(name, inst_ref));
    }

    for(uint32_t i = orig_material_count; i < new_material_count; ++i)
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

void Scene::createSkybox(GfxContext const& gfx, const char* textureFile)
{
	Shape shape = Shapes::skybox();

	skybox = new Skybox();

	skybox->indices = shape.indices;
	skybox->vertices = shape.vertices;
	skybox->indexBuffer = gfxCreateBuffer<uint32_t>(gfx, shape.indexCount, shape.indices.data());
	skybox->vertexBuffer = gfxCreateBuffer<Vertex>(gfx, shape.vertexCount, shape.vertices.data());

	gfxSceneImport(gfxScene, textureFile);
	GfxConstRef<GfxImage> imgRef = gfxSceneGetImageHandle(gfxScene, gfxSceneGetImageCount(gfxScene) - 1);
	uint32_t const mipCount = gfxCalculateMipCount(imgRef->width, imgRef->height);
	skybox->material.texture = gfxCreateTexture2D(gfx, imgRef->width, imgRef->height, imgRef->format, mipCount);
	GfxBuffer uploadBuffer = gfxCreateBuffer(gfx, imgRef->width * imgRef->height * imgRef->channel_count, imgRef->data.data());
    gfxCommandCopyBufferToTexture(gfx, skybox->material.texture, uploadBuffer);
	gfxCommandGenerateMips(gfx, skybox->material.texture);
    gfxDestroyBuffer(gfx, uploadBuffer);
	skybox->material.hasTexture = true;
}

void Scene::destroy(GfxContext const& gfx)
{
    for(uint32_t i = 0; i < indexBuffers.size(); ++i)
        gfxDestroyBuffer(gfx, indexBuffers.data()[i]);
    for(uint32_t i = 0; i < vertexBuffers.size(); ++i)
        gfxDestroyBuffer(gfx, vertexBuffers.data()[i]);
    for(uint32_t i = 0; i < albedoBuffers.size(); ++i)
        gfxDestroyTexture(gfx, albedoBuffers.data()[i]);

	for (GameObject* obj : gameObjects)
	{
		Mesh* mesh = dynamic_cast<Mesh*>(obj);
		if (mesh)
		{
			gfxDestroyBuffer(gfx, mesh->indexBuffer);
			gfxDestroyBuffer(gfx, mesh->vertexBuffer);
			gfxDestroyTexture(gfx, mesh->material.texture);
		}

		if (obj != nullptr)
		{
			delete obj->name;
			delete obj;
		}
	}

	if (skybox != nullptr)
	{
		gfxDestroyBuffer(gfx, skybox->indexBuffer);
		gfxDestroyBuffer(gfx, skybox->vertexBuffer);
		gfxDestroyTexture(gfx, skybox->material.texture);
	}

	gfxDestroyScene(gfxScene);

	gfxDestroyTexture(gfx, depthBuffer);
	gfxDestroyTexture(gfx, colorBuffer);
	gfxDestroyKernel(gfx, litKernel);
	gfxDestroyKernel(gfx, litResolveKernel);
	gfxDestroyProgram(gfx, litProgram);
	gfxDestroyKernel(gfx, skyboxKernel);
	gfxDestroyKernel(gfx, skyboxResolveKernel);
	gfxDestroyProgram(gfx, skyboxProgram);
	gfxDestroySamplerState(gfx, textureSampler);
}
