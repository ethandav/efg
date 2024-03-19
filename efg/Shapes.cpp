#include "Shapes.h"

void Shapes::scale(Shape& shape, float scaleFactor)
{
    for (auto& vertex : shape.vertices)
    {
        vertex.position *= scaleFactor;
    }
}

Shape Shapes::cube()
{
    Shape cube;
    cube.vertexCount = 24;
    cube.indexCount = 36;
    cube.vertices = std::vector<GfxVertex>(cube.vertexCount);
    cube.indices = std::vector<uint32_t>(cube.indexCount);

    cube.indices = {
         2,  1,  0, // Top
         3,  1,  2,
         6,  5,  4, // Back
         7,  5,  6,
        10,  9,  8, // Right
        11,  9, 10,
        14, 13, 12, // Front
        15, 13, 14,
        18, 17, 16, // Left
        19, 17, 18,
        22, 21, 20, // Bottom
        23, 21, 22
    };

    cube.vertices[0].position  = glm::vec3(-0.5f,  0.5f, -0.5f); //Top
    cube.vertices[1].position  = glm::vec3( 0.5f,  0.5f, -0.5f);
    cube.vertices[2].position  = glm::vec3(-0.5f,  0.5f,  0.5f);
    cube.vertices[3].position  = glm::vec3( 0.5f,  0.5f,  0.5f);
    cube.vertices[4].position  = glm::vec3(-0.5f,  0.5f,  0.5f); //Front
    cube.vertices[5].position  = glm::vec3( 0.5f,  0.5f,  0.5f);
    cube.vertices[6].position  = glm::vec3(-0.5f, -0.5f,  0.5f);
    cube.vertices[7].position  = glm::vec3( 0.5f, -0.5f,  0.5f);
    cube.vertices[8].position  = glm::vec3( 0.5f,  0.5f,  0.5f); //Right
    cube.vertices[9].position  = glm::vec3( 0.5f,  0.5f, -0.5f);
    cube.vertices[10].position = glm::vec3( 0.5f, -0.5f,  0.5f);
    cube.vertices[11].position = glm::vec3( 0.5f, -0.5f, -0.5f);
    cube.vertices[12].position = glm::vec3( 0.5f,  0.5f, -0.5f); //Back
    cube.vertices[13].position = glm::vec3(-0.5f,  0.5f, -0.5f);
    cube.vertices[14].position = glm::vec3( 0.5f, -0.5f, -0.5f);
    cube.vertices[15].position = glm::vec3(-0.5f, -0.5f, -0.5f);
    cube.vertices[16].position = glm::vec3(-0.5f,  0.5f, -0.5f); //Left
    cube.vertices[17].position = glm::vec3(-0.5f,  0.5f,  0.5f);
    cube.vertices[18].position = glm::vec3(-0.5f, -0.5f, -0.5f);
    cube.vertices[19].position = glm::vec3(-0.5f, -0.5f,  0.5f);
    cube.vertices[20].position = glm::vec3(-0.5f, -0.5f,  0.5f); //Bottom
    cube.vertices[21].position = glm::vec3( 0.5f, -0.5f,  0.5f);
    cube.vertices[22].position = glm::vec3(-0.5f, -0.5f, -0.5f);
    cube.vertices[23].position = glm::vec3( 0.5f, -0.5f, -0.5f);

    cube.vertices[0].normal  = glm::vec3( 0.0f,  1.0f,  0.0f); //Top
    cube.vertices[1].normal  = glm::vec3( 0.0f,  1.0f,  0.0f);
    cube.vertices[2].normal  = glm::vec3( 0.0f,  1.0f,  0.0f); 
    cube.vertices[3].normal  = glm::vec3( 0.0f,  1.0f,  0.0f);
    cube.vertices[4].normal  = glm::vec3( 0.0f,  0.0f,  1.0f); //Front
    cube.vertices[5].normal  = glm::vec3( 0.0f,  0.0f,  1.0f);
    cube.vertices[6].normal  = glm::vec3( 0.0f,  0.0f,  1.0f);
    cube.vertices[7].normal  = glm::vec3( 0.0f,  0.0f,  1.0f);
    cube.vertices[8].normal  = glm::vec3( 1.0f,  0.0f,  0.0f); //Right
    cube.vertices[9].normal  = glm::vec3( 1.0f,  0.0f,  0.0f);
    cube.vertices[10].normal = glm::vec3( 1.0f,  0.0f,  0.0f);
    cube.vertices[11].normal = glm::vec3( 1.0f,  0.0f,  0.0f);
    cube.vertices[12].normal = glm::vec3( 0.0f,  0.0f, -1.0f); //Back
    cube.vertices[13].normal = glm::vec3( 0.0f,  0.0f, -1.0f);
    cube.vertices[14].normal = glm::vec3( 0.0f,  0.0f, -1.0f);
    cube.vertices[15].normal = glm::vec3( 0.0f,  0.0f, -1.0f);
    cube.vertices[16].normal = glm::vec3(-1.0f,  0.0f,  0.0f); //Left
    cube.vertices[17].normal = glm::vec3(-1.0f,  0.0f,  0.0f);
    cube.vertices[18].normal = glm::vec3(-1.0f,  0.0f,  0.0f);
    cube.vertices[19].normal = glm::vec3(-1.0f,  0.0f,  0.0f);
    cube.vertices[20].normal = glm::vec3( 0.0f, -1.0f,  0.0f); //Bottom
    cube.vertices[21].normal = glm::vec3( 0.0f, -1.0f,  0.0f);
    cube.vertices[22].normal = glm::vec3( 0.0f, -1.0f,  0.0f);
    cube.vertices[23].normal = glm::vec3( 0.0f, -1.0f,  0.0f);

    // Top face
    cube.vertices[0].uv = glm::vec2(0.0f, 1.0f);
    cube.vertices[1].uv = glm::vec2(1.0f, 1.0f);
    cube.vertices[2].uv = glm::vec2(0.0f, 0.0f);
    cube.vertices[3].uv = glm::vec2(1.0f, 0.0f);
    // Front face
    cube.vertices[4].uv = glm::vec2(0.0f, 1.0f);
    cube.vertices[5].uv = glm::vec2(1.0f, 1.0f);
    cube.vertices[6].uv = glm::vec2(0.0f, 0.0f);
    cube.vertices[7].uv = glm::vec2(1.0f, 0.0f);
    // Right face
    cube.vertices[8].uv = glm::vec2(0.0f, 1.0f);
    cube.vertices[9].uv = glm::vec2(1.0f, 1.0f);
    cube.vertices[10].uv = glm::vec2(0.0f, 0.0f);
    cube.vertices[11].uv = glm::vec2(1.0f, 0.0f);
    // Back face
    cube.vertices[12].uv = glm::vec2(0.0f, 1.0f);
    cube.vertices[13].uv = glm::vec2(1.0f, 1.0f);
    cube.vertices[14].uv = glm::vec2(0.0f, 0.0f);
    cube.vertices[15].uv = glm::vec2(1.0f, 0.0f);
    // Left face
    cube.vertices[16].uv = glm::vec2(0.0f, 1.0f);
    cube.vertices[17].uv = glm::vec2(1.0f, 1.0f);
    cube.vertices[18].uv = glm::vec2(0.0f, 0.0f);
    cube.vertices[19].uv = glm::vec2(1.0f, 0.0f);
    // Bottom face
    cube.vertices[20].uv = glm::vec2(0.0f, 1.0f);
    cube.vertices[21].uv = glm::vec2(1.0f, 1.0f);
    cube.vertices[22].uv = glm::vec2(0.0f, 0.0f);
    cube.vertices[23].uv = glm::vec2(1.0f, 0.0f);

    return cube;
}

Shape Shapes::sphere()
{
    Shape sphere;
    sphere.indexCount = 5000;
    sphere.vertexCount = 1000;

    size_t tessellation = 20;

    const size_t verticalSegments = tessellation;
    const size_t horizontalSegments = tessellation * 2;

    sphere.vertices.reserve(verticalSegments * horizontalSegments);

    float diameter = 2;
    const float radius = diameter / 2;

    for (size_t i = 0; i <= verticalSegments; i++)
    {
        const float v = 1 - float(i) / float(verticalSegments);

        const float latitude = (float(i) * glm::pi<float>() / float(verticalSegments)) - glm::half_pi<float>();
        float dy, dxz;

        dy = glm::sin(latitude);
        dxz = glm::cos(latitude);

        for (size_t j = 0; j <= horizontalSegments; j++)
        {
            const float u = float(j) / float(horizontalSegments);

            const float longitude = float(j) * glm::two_pi<float>() / float(horizontalSegments);
            float dx, dz;

            dx = glm::sin(longitude);
            dz = glm::cos(longitude);

            dx *= dxz;
            dz *= dxz;

            GfxVertex vertex;
            vertex.position.x = dx * radius;
            vertex.position.y = dy * radius;
            vertex.position.z = dz * radius;

            vertex.uv.x = u;
            vertex.uv.y = v;

            vertex.normal = glm::normalize(vertex.position);

            sphere.vertices.push_back(vertex);
        }
    }

    const size_t stride = horizontalSegments + 1;

    for (size_t i = 0; i < verticalSegments; i++)
    {
        for (size_t j = 0; j <= horizontalSegments; j++)
        {
            const size_t nextI = i + 1;
            const size_t nextJ = (j + 1) % stride;

            sphere.indices.push_back( i * stride + j);
            sphere.indices.push_back( i * stride + nextJ);
            sphere.indices.push_back( nextI * stride + j);

            sphere.indices.push_back( i * stride + nextJ);
            sphere.indices.push_back( nextI * stride + nextJ);
            sphere.indices.push_back( nextI * stride + j);
        }
    }

    return sphere;
}
