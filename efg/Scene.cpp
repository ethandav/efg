#include "Scene.h"

Scene::Scene()
{

}

void Scene::initialize(const GfxContext& gfx)
{
	m_gfx = &gfx;
	gfxScene = gfxCreateScene();

	LoadSceneFromFile("assets/sponza.obj");
	//GfxRef<GfxInstance> obj1 = AddPrimitiveToScene(Shapes::SPHERE, "assets/textures/earth.jpeg");
	//GfxRef<GfxInstance> obj2 = AddPrimitiveToScene(Shapes::SPHERE, "assets/textures/moon.jpg");
	//GfxRef<GfxInstance> obj3 = AddPrimitiveToScene(Shapes::SPHERE, "assets/textures/sun.png");

	//obj2->transform = CreateTransformationMatrix(
	//	glm::vec3(1.0, 1.0f, -15.0f),
	//	glm::vec3(0.0f, 0.0f, 0.0f),
	//	glm::vec3(.5f, .5f, .5f)
	//);

	//obj3->transform = CreateTransformationMatrix(
	//	glm::vec3(0.0, 0.0f, -1000.0f),
	//	glm::vec3(0.0f, 0.0f, 0.0f),
	//	glm::vec3(20.0f, 20.0f, 20.0f)
	//);
}

void Scene::update(GfxProgram const& program) const
{
	GfxContext gfx = *m_gfx;

	for (uint32_t i = 0; i < gfxSceneGetInstanceCount(gfxScene); ++i)
	{
		GfxInstance const& instance = gfxSceneGetInstances(gfxScene)[i];

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
}

GfxRef<GfxInstance> Scene::AddPrimitiveToScene(const Shapes::Types type, const char* textureFile)
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

void Scene::destroy()
{
	const GfxContext gfx = *m_gfx;

    for(uint32_t i = 0; i < indexBuffers.size(); ++i)
        gfxDestroyBuffer(gfx, indexBuffers.data()[i]);
    for(uint32_t i = 0; i < vertexBuffers.size(); ++i)
        gfxDestroyBuffer(gfx, vertexBuffers.data()[i]);
    for(uint32_t i = 0; i < albedoBuffers.size(); ++i)
        gfxDestroyTexture(gfx, albedoBuffers.data()[i]);

	gfxDestroyScene(gfxScene);
}
