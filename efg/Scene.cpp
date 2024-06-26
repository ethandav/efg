#include "Scene.h"
#include <random>

void Scene::initialize(const GfxContext& gfx)
{
	gfxScene = gfxCreateScene();

	lightingManager.initialize(gfx);

	//cam = CreateFlyCamera(gfx, glm::vec3(0.0f, 5.0f, 10.0f), glm::vec3(3.0f, 5.0f, 0.0f));
	cam = CreateFlyCamera(gfx, glm::vec3(0.0f, 5.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	colorBuffer = gfxCreateTexture2D(gfx, DXGI_FORMAT_R16G16B16A16_FLOAT);
    depthBuffer = gfxCreateTexture2D(gfx, DXGI_FORMAT_D32_FLOAT);

	gfxDrawStateSetColorTarget(drawState, 0, colorBuffer.getFormat());
	gfxDrawStateSetDepthStencilTarget(drawState, depthBuffer.getFormat());
	gfxDrawStateSetDepthFunction(drawState, D3D12_COMPARISON_FUNC_LESS);
	gfxDrawStateEnableAlphaBlending(drawState);

	litProgram = gfxCreateProgram(gfx, "lit");
	litKernel = gfxCreateGraphicsKernel(gfx, litProgram, drawState);
	litResolveKernel = gfxCreateGraphicsKernel(gfx, litProgram, "resolve");

	waterProgram = gfxCreateProgram(gfx, "water");
	waterKernel = gfxCreateGraphicsKernel(gfx, waterProgram, drawState);
	waterResolveKernel = gfxCreateGraphicsKernel(gfx, waterProgram, "resolve");

	skyboxProgram = gfxCreateProgram(gfx, "sky");
	skyboxKernel = gfxCreateGraphicsKernel(gfx, skyboxProgram, drawState);

	textureSampler = gfxCreateSamplerState(gfx, D3D12_FILTER_ANISOTROPIC, D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_WRAP);

	gfxProgramSetParameter(gfx, litProgram, "TextureSampler", textureSampler);
	gfxProgramSetParameter(gfx, litProgram, "ColorBuffer", colorBuffer);

	gfxProgramSetParameter(gfx, waterProgram, "TextureSampler", textureSampler);
	gfxProgramSetParameter(gfx, waterProgram, "ColorBuffer", colorBuffer);

	gfxProgramSetParameter(gfx, skyboxProgram, "TextureSampler", textureSampler);
	gfxProgramSetParameter(gfx, skyboxProgram, "ColorBuffer", colorBuffer);


    loadScene(gfx);
}

void Scene::loadScene(const GfxContext& gfx)
{
	const char* textureFiles[6] = {
		"assets/skybox/right.png", // Right
		"assets/skybox/left.png", // Left
		"assets/skybox/top.png", // Top
		"assets/skybox/bottom.png", // Down
		"assets/skybox/front.png", // Front
		"assets/skybox/back.png"  // Back
	};

	createSkybox(gfx, textureFiles);

	lightingManager.addDirectionalLight(gfx);
	lightingManager.addPointLight(gfx);

	//Mesh* plane = AddPrimitive(
	//	gfx,
	//	"Plane",
	//	Shapes::PLANE,
	//	false,
	//	nullptr,
	//	nullptr,
	//	glm::vec3(0.0, 0.0f, 0.0f),
	//	glm::vec3(0.0, 0.0f, 0.0f),
	//	glm::vec3(100.0, 100.0f, 100.0f)
	//);

	Mesh* obj1 = AddPrimitive(
		gfx,
		"Sphere 1",
		Shapes::GRID,
		false,
		"assets/textures/water.jpg",
		nullptr,
		glm::vec3(0.0, 0.0f, 0.0f)
	);
	obj1->material.cBuffer.properties.ambient = glm::vec4(0.1f, 0.3f, 1.0f, 0.0f);
	waterSurfaces.push_back(obj1);

	Mesh* obj2 = AddPrimitive(
		gfx,
		"Sphere 1",
		Shapes::SPHERE,
		false,
		nullptr,
		nullptr,
		glm::vec3(0.0, -2.0f, 0.0f)
	);
	gameObjects.push_back(obj2);

	Mesh* obj3 = AddPrimitive(
		gfx,
		"wall 1",
		Shapes::SQUARE,
		false,
		nullptr,
		nullptr,
		glm::vec3(0.0, -1.0f, -5.0f),
		glm::vec3(0.0, 0.0f, 0.0f),
		glm::vec3(10.0, 5.0f, 10.0f)
	);
	gameObjects.push_back(obj3);

	Mesh* obj4 = AddPrimitive(
		gfx,
		"wall 2",
		Shapes::SQUARE,
		false,
		nullptr,
		nullptr,
		glm::vec3(0.0, -1.0f, 5.0f),
		glm::vec3(0.0, 0.0f, 0.0f),
		glm::vec3(10.0, 5.0f, 10.0f)
	);
	gameObjects.push_back(obj4);

	Mesh* obj5 = AddPrimitive(
		gfx,
		"wall 3",
		Shapes::SQUARE,
		false,
		nullptr,
		nullptr,
		glm::vec3(-5.0, -1.0f, 0.0f),
		glm::vec3(0.0, 90.0f, 0.0f),
		glm::vec3(10.0, 5.0f, 10.0f)
	);
	gameObjects.push_back(obj5);

	Mesh* obj6 = AddPrimitive(
		gfx,
		"wall 3",
		Shapes::SQUARE,
		false,
		nullptr,
		nullptr,
		glm::vec3(5.0, -1.0f, 0.0f),
		glm::vec3(0.0, -90.0f, 0.0f),
		glm::vec3(10.0, 5.0f, 10.0f)
	);
	gameObjects.push_back(obj6);

	Mesh* obj7 = AddPrimitive(
		gfx,
		"wall 3",
		Shapes::PLANE,
		false,
		nullptr,
		nullptr,
		glm::vec3(0.0, -3.0f, 0.0f),
		glm::vec3(0.0, 0.0f, 0.0f),
		glm::vec3(2.0, 2.0f, 2.0f)
	);
	gameObjects.push_back(obj7);

	//Mesh* obj3 = AddPrimitive(
	//	gfx,
	//	"Sphere 3",
	//	Shapes::SPHERE,
	//	false,
	//	nullptr,
	//	nullptr,
	//	glm::vec3(0.0, 1.0f, -3.0f)
	//);

	//std::mt19937 rng(std::random_device{}());
	//std::uniform_real_distribution<float> dist(-50.0f, 50.0f);

	//std::vector<glm::mat4> matrices;
	//matrices.reserve(2000);
	//for (int i = 0; i < 2000; i++)
	//{
	//	matrices.push_back(CreateModelMatrix(glm::vec3(dist(rng), dist(rng), dist(rng)), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
	//}

	//AddPrimitiveInstanced(gfx, "Sphere", Shapes::SPHERE, 2000, matrices);

	//LoadSceneFromFile(gfx, "assets/sponza.obj");
}

void Scene::update(GfxContext const& gfx, GfxWindow const& window, double deltaTime)
{    
	totalTime += deltaTime;
	UpdateFlyCamera(gfx, window, cam);

	gfxCommandBindColorTarget(gfx, 0, colorBuffer);
	gfxCommandBindDepthStencilTarget(gfx, depthBuffer);
	gfxCommandClearTexture(gfx, colorBuffer);
	gfxCommandClearTexture(gfx, depthBuffer);
	gfxProgramSetParameter(gfx, litProgram, "g_ViewProjection", cam.view_proj);
	gfxProgramSetParameter(gfx, litProgram, "viewPos", cam.eye);
	gfxProgramSetParameter(gfx, litProgram, "time", totalTime);
	gfxProgramSetParameter(gfx, waterProgram, "g_ViewProjection", cam.view_proj);
	gfxProgramSetParameter(gfx, waterProgram, "viewPos", cam.eye);
	gfxProgramSetParameter(gfx, waterProgram, "time", totalTime);
	gfxProgramSetParameter(gfx, skyboxProgram, "g_View", glm::mat4(glm::mat3(cam.view)));
	gfxProgramSetParameter(gfx, skyboxProgram, "g_Projection", cam.proj);

	updateSkybox(gfx);
	lightingManager.update(gfx, litProgram, deltaTime);
	lightingManager.updateDirectionals = true;
	lightingManager.updatePoints = true;
	lightingManager.update(gfx, waterProgram, deltaTime);
    updateGameObjects(gfx);
	updateWaterSurfaces(gfx);

	gfxCommandBindKernel(gfx, waterResolveKernel);
	gfxCommandDraw(gfx, 3);

	gfxCommandBindKernel(gfx, litResolveKernel);
	gfxCommandDraw(gfx, 3);

}

std::vector<GameObject*>* Scene::getGameObjects()
{
	return &gameObjects;
}

void Scene::updateGameObjects(GfxContext const& gfx)
{
	gfxCommandBindKernel(gfx, litKernel);

    for (GameObject* obj : gameObjects)
    {
        if (obj == nullptr)
            continue;

		if (obj->update)
		{
			obj->modelMatrix = CreateModelMatrix(obj->position, obj->rotation, obj->scale);
			obj->update = false;
		}

		obj->draw(gfx, litProgram);
		if (obj->reference)
		{
			DrawInstanced(gfx, obj);
		}
    }
}

void Scene::updateWaterSurfaces(GfxContext const& gfx)
{
	gfxCommandBindKernel(gfx, waterKernel);

    for (GameObject* obj : waterSurfaces)
    {
        if (obj == nullptr)
            continue;

		if (obj->update)
		{
			obj->modelMatrix = CreateModelMatrix(obj->position, obj->rotation, obj->scale);
			obj->update = false;
		}

		obj->draw(gfx, waterProgram);
		if (obj->reference)
		{
			DrawInstanced(gfx, obj);
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
	if (skybox != nullptr)
	{
		gfxCommandBindKernel(gfx, skyboxKernel);
		gfxProgramSetParameter(gfx, skyboxProgram, "transform", glm::mat4(1.0f));
		gfxProgramSetParameter(gfx, skyboxProgram, "CubeMap", skybox->textureCube);
		gfxCommandBindIndexBuffer(gfx, skybox->indexBuffer);
		gfxCommandBindVertexBuffer(gfx, skybox->vertexBuffer);
		gfxCommandDrawIndexed(gfx, (uint32_t)skybox->indices.size());
	}
}

void Scene::AddPrimitiveInstanced(GfxContext const& gfx, const char* name, const Shapes::Types type, uint32_t instances,
	std::vector<glm::mat4> matrices, const char* textureFile, const char* specularMap)
{
	Shape shape = Shapes::getShape(type);
	Mesh* newMesh = new Mesh();

	newMesh->modelMatrices = matrices;
	newMesh->instances = instances;
	newMesh->indices = shape.indices;
	newMesh->vertices = shape.vertices;
	newMesh->indexBuffer = gfxCreateBuffer<uint32_t>(gfx, shape.indexCount, shape.indices.data());
	newMesh->vertexBuffer = gfxCreateBuffer<Vertex>(gfx, shape.vertexCount, shape.vertices.data());
	newMesh->instanceBuffer = gfxCreateBuffer<glm::mat4>(gfx, instances, newMesh->modelMatrices.data());

	if (textureFile != nullptr)
	{
		gfxSceneImport(gfxScene, textureFile);
		GfxConstRef<GfxImage> imgRef = gfxSceneGetImageHandle(gfxScene, gfxSceneGetImageCount(gfxScene) - 1);
		uint32_t const mipCount = gfxCalculateMipCount(imgRef->width, imgRef->height);
		newMesh->material.diffuseMap= gfxCreateTexture2D(gfx, imgRef->width, imgRef->height, imgRef->format, mipCount);
		GfxBuffer uploadBuffer = gfxCreateBuffer(gfx, imgRef->width * imgRef->height * imgRef->channel_count, imgRef->data.data());
		gfxCommandCopyBufferToTexture(gfx, newMesh->material.diffuseMap, uploadBuffer);
		gfxCommandGenerateMips(gfx, newMesh->material.diffuseMap);
		gfxDestroyBuffer(gfx, uploadBuffer);
		newMesh->material.useDiffuseMap = true;
	}

	if (specularMap != nullptr)
	{
		gfxSceneImport(gfxScene, specularMap);
		GfxConstRef<GfxImage> imgRef = gfxSceneGetImageHandle(gfxScene, gfxSceneGetImageCount(gfxScene) - 1);
		uint32_t const mipCount = gfxCalculateMipCount(imgRef->width, imgRef->height);
		newMesh->material.specularMap = gfxCreateTexture2D(gfx, imgRef->width, imgRef->height, imgRef->format, mipCount);
		GfxBuffer uploadBuffer = gfxCreateBuffer(gfx, imgRef->width * imgRef->height * imgRef->channel_count, imgRef->data.data());
		gfxCommandCopyBufferToTexture(gfx, newMesh->material.specularMap, uploadBuffer);
		gfxCommandGenerateMips(gfx, newMesh->material.specularMap);
		gfxDestroyBuffer(gfx, uploadBuffer);
		newMesh->material.useSpecMap = true;
	}

	char* objName = nullptr;
	if (name == nullptr)
	{
		std::string newName = std::string("Object ") + std::to_string(gameObjects.size() + 1);
		objName = new char[strlen(newName.c_str()) + 1];
		strcpy_s(objName, strlen(newName.c_str()) + 1, newName.c_str());
	}
	else
	{
		objName = new char[strlen(name) + 1];
		strcpy_s(objName, strlen(name) + 1, name);
	}

	newMesh->name = objName;
}

Mesh* Scene::AddPrimitive(GfxContext const& gfx, const char* name, const Shapes::Types type, bool atCam,
	const char* textureFile, const char* specularMap,
	glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale)
{
	Shape shape = Shapes::getShape(type);

	Mesh* newMesh = new Mesh();

	if (atCam && translation == glm::vec3(0.0f))
	{
		translation = cam.center;
	}

	newMesh->position = translation;
	newMesh->rotation = rotation;
	newMesh->scale = scale;

	std::vector<glm::mat4> matrix = {
		CreateModelMatrix(translation, rotation, scale)
	};

	newMesh->indices = shape.indices;
	newMesh->vertices = shape.vertices;
	newMesh->indexBuffer = gfxCreateBuffer<uint32_t>(gfx, shape.indexCount, shape.indices.data());
	newMesh->vertexBuffer = gfxCreateBuffer<Vertex>(gfx, shape.vertexCount, shape.vertices.data());
	newMesh->instanceBuffer = gfxCreateBuffer<glm::mat4>(gfx, 1, matrix.data());

	if (textureFile != nullptr)
	{
		gfxSceneImport(gfxScene, textureFile);
		GfxConstRef<GfxImage> imgRef = gfxSceneGetImageHandle(gfxScene, gfxSceneGetImageCount(gfxScene) - 1);
		uint32_t const mipCount = gfxCalculateMipCount(imgRef->width, imgRef->height);
		newMesh->material.diffuseMap= gfxCreateTexture2D(gfx, imgRef->width, imgRef->height, imgRef->format, mipCount);
		GfxBuffer uploadBuffer = gfxCreateBuffer(gfx, imgRef->width * imgRef->height * imgRef->channel_count, imgRef->data.data());
		gfxCommandCopyBufferToTexture(gfx, newMesh->material.diffuseMap, uploadBuffer);
		gfxCommandGenerateMips(gfx, newMesh->material.diffuseMap);
		gfxDestroyBuffer(gfx, uploadBuffer);
		newMesh->material.useDiffuseMap = true;
	}

	if (specularMap != nullptr)
	{
		gfxSceneImport(gfxScene, specularMap);
		GfxConstRef<GfxImage> imgRef = gfxSceneGetImageHandle(gfxScene, gfxSceneGetImageCount(gfxScene) - 1);
		uint32_t const mipCount = gfxCalculateMipCount(imgRef->width, imgRef->height);
		newMesh->material.specularMap = gfxCreateTexture2D(gfx, imgRef->width, imgRef->height, imgRef->format, mipCount);
		GfxBuffer uploadBuffer = gfxCreateBuffer(gfx, imgRef->width * imgRef->height * imgRef->channel_count, imgRef->data.data());
		gfxCommandCopyBufferToTexture(gfx, newMesh->material.specularMap, uploadBuffer);
		gfxCommandGenerateMips(gfx, newMesh->material.specularMap);
		gfxDestroyBuffer(gfx, uploadBuffer);
		newMesh->material.useSpecMap = true;
	}

	char* objName = nullptr;
	if (name == nullptr)
	{
		std::string newName = std::string("Object ") + std::to_string(gameObjects.size() + 1);
		objName = new char[strlen(newName.c_str()) + 1];
		strcpy_s(objName, strlen(newName.c_str()) + 1, newName.c_str());
	}
	else
	{
		objName = new char[strlen(name) + 1];
		strcpy_s(objName, strlen(name) + 1, name);
	}

	newMesh->name = objName;

	return newMesh;
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

void Scene::createSkybox(GfxContext const& gfx, const char* textureFiles[6])
{
	bool textureCreated = false;
	Shape shape = Shapes::getShape(Shapes::SKYBOX);
	skybox = new Skybox();

	skybox->indices = shape.indices;
	skybox->vertices = shape.vertices;
	skybox->indexBuffer = gfxCreateBuffer<uint32_t>(gfx, shape.indexCount, shape.indices.data());
	skybox->vertexBuffer = gfxCreateBuffer<Vertex>(gfx, shape.vertexCount, shape.vertices.data());

	for (uint32_t i = 0; i < 6; ++i)
	{
		gfxSceneImport(gfxScene, textureFiles[i]);
		GfxConstRef<GfxImage> imgRef = gfxSceneGetImageHandle(gfxScene, gfxSceneGetImageCount(gfxScene) - 1);
		GfxBuffer uploadBuffer = gfxCreateBuffer(gfx, imgRef->width * imgRef->height * imgRef->channel_count * imgRef->bytes_per_channel, imgRef->data.data());
		if (!textureCreated)
		{
			skybox->textureCube = gfxCreateTextureCube(gfx, imgRef->width, DXGI_FORMAT_R8G8B8A8_UNORM, gfxCalculateMipCount(imgRef->width, imgRef->height));
			textureCreated = true;
		}
		gfxCommandCopyBufferToCubeFace(gfx, skybox->textureCube, uploadBuffer, i);
		gfxDestroyBuffer(gfx, uploadBuffer);
	}
	gfxCommandGenerateMips(gfx, skybox->textureCube);
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
		if (obj != nullptr)
		{
			obj->destroy(gfx);
			delete obj->name;
			delete obj;
		}
	}

	if (skybox != nullptr)
	{
		gfxDestroyTexture(gfx, skybox->textureCube);
		skybox->destroy(gfx);
	}

	lightingManager.destroy(gfx);

	gfxDestroyScene(gfxScene);

	gfxDestroyTexture(gfx, depthBuffer);
	gfxDestroyTexture(gfx, colorBuffer);
	gfxDestroyKernel(gfx, litKernel);
	gfxDestroyKernel(gfx, litResolveKernel);
	gfxDestroyProgram(gfx, litProgram);
	gfxDestroyKernel(gfx, waterKernel);
	gfxDestroyKernel(gfx, waterResolveKernel);
	gfxDestroyProgram(gfx, waterProgram);
	gfxDestroyKernel(gfx, skyboxKernel);
	gfxDestroyKernel(gfx, skyboxResolveKernel);
	gfxDestroyProgram(gfx, skyboxProgram);
	gfxDestroySamplerState(gfx, textureSampler);
}
