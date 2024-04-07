#include "Scene.h"

void Scene::initialize(const GfxContext& gfx)
{
	m_gfx = &gfx;
	gfxScene = gfxCreateScene();

	addLight("Light 1");
	AddPrimitive("Earth", Shapes::SPHERE, "assets/textures/earth.jpeg");

	AddPrimitive(
		"Moon",
		Shapes::SPHERE,
		"assets/textures/moon.jpg",
		glm::vec3(1.0, 1.0f, -15.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(.5f, .5f, .5f)
	);

	AddPrimitive(
		"Sun",
		Shapes::SPHERE,
		"assets/textures/sun.png",
		glm::vec3(0.0, 0.0f, -1000.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(20.0f, 20.0f, 20.0f)
	);

	//LoadSceneFromFile("assets/sponza.obj");
}

void Scene::update(GfxProgram const& program) const
{
	GfxContext gfx = *m_gfx;
	ImGui::Begin("Game Objects");

	for (GameObject* obj : gameObjects)
	{
        if (ImGui::TreeNode(obj->name)) {
			ImGui::InputFloat3("Position", &obj->position[0], "%.3f");
            ImGui::TreePop();
        }

		if (dynamic_cast<Instance*>(obj))
		{
			GfxInstance const& instance = gfxSceneGetInstances(gfxScene)[(dynamic_cast<Instance*>(obj))->reference.getIndex()];

			gfxProgramSetParameter(gfx, program, "transform", instance.transform);

			if (instance.material)
			{
				gfxProgramSetParameter(gfx, program, "AlbedoBuffer", albedoBuffers[instance.material]);
			}
			else
			{
				gfxProgramSetParameter(gfx, program, "AlbedoBuffer", GfxTexture());
			}
			gfxCommandBindIndexBuffer(gfx, indexBuffers[instance.mesh]);
			gfxCommandBindVertexBuffer(gfx, vertexBuffers[instance.mesh]);
			gfxCommandDrawIndexed(gfx, (uint32_t)instance.mesh->indices.size());
		}

		if(dynamic_cast<Light*>(obj))
		{
			Light* light = dynamic_cast<Light*>(obj);
			gfxProgramSetParameter(gfx, program, "lightColor", light->lightColor);
			gfxProgramSetParameter(gfx, program, "lightPosition", light->lightPosition);
			gfxProgramSetParameter(gfx, program, "lightIntensity", light->lightIntensity);
			gfxProgramSetParameter(gfx, program, "specStrength", light->specStrength);
			gfxProgramSetParameter(gfx, program, "shininess", light->shininess);

			//ImGui::Separator();
			//ImGui::SliderFloat("Ambient Light Intensity", &light->lightIntensity, 0, 5);
			//ImGui::Separator();
			//ImGui::SliderFloat("Specular Strength", &light->specStrength, 0, 5);
			//ImGui::Separator();
			//ImGui::InputInt("Shininess", &light->shininess, 2, 256);
			//ImGui::Separator();
			//ImGui::Text("Light Position");
			//ImGui::SliderFloat("x", &light->lightPosition.x, -20, 20);
			//ImGui::SliderFloat("y", &light->lightPosition.y, -20, 20);
			//ImGui::SliderFloat("z", &light->lightPosition.z, -20, 20);
			//ImGui::Separator();
			//ImGui::ColorPicker3("Light Source Color", light->lightColor);
			//ImGui::Separator();
		}
	}

    ImGui::End();
}

void Scene::addLight(const char* name)
{
	GfxRef<GfxInstance> reference = gfxSceneCreateInstance(gfxScene);
	char* lightName = new char[strlen(name) + 1];
	strcpy_s(lightName, strlen(name) + 1, name);
	Light* light = new Light(lightName, reference);
	gameObjects.push_back(light);
}

void Scene::AddPrimitive(const char* name, const Shapes::Types type, const char* textureFile,
	glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale)
{
	const GfxContext gfx = *m_gfx;
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

	char* objName = new char[strlen(name) + 1];
	strcpy_s(objName, strlen(name) + 1, name);
	gameObjects.push_back(new Instance(objName, newInstance));
}

glm::mat4 Scene::CreateTransformationMatrix(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale)
{
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), translation);
	glm::mat4 rotationMatrix = glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);
	glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), scale);
	return translationMatrix * rotationMatrix * scalingMatrix;
}

void Scene::LoadSceneFromFile(const char* assetFile)
{
	const GfxContext gfx = *m_gfx;
    uint32_t const orig_instance_count = gfxSceneGetInstanceCount(gfxScene);
    uint32_t const orig_mesh_count     = gfxSceneGetMeshCount(gfxScene);
    uint32_t const orig_material_count = gfxSceneGetMaterialCount(gfxScene);

	gfxSceneImport(gfxScene, assetFile);   // import our scene

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

		gameObjects.push_back(new Instance(name, inst_ref));
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

void Scene::destroy()
{
	const GfxContext gfx = *m_gfx;

    for(uint32_t i = 0; i < indexBuffers.size(); ++i)
        gfxDestroyBuffer(gfx, indexBuffers.data()[i]);
    for(uint32_t i = 0; i < vertexBuffers.size(); ++i)
        gfxDestroyBuffer(gfx, vertexBuffers.data()[i]);
    for(uint32_t i = 0; i < albedoBuffers.size(); ++i)
        gfxDestroyTexture(gfx, albedoBuffers.data()[i]);

	for (GameObject* obj : gameObjects)
	{
		delete obj->name;
		delete obj;
	}

	gfxDestroyScene(gfxScene);
}
