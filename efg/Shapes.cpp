#include "Shapes.h"


Shape square()
{
    Shape square;
    square.vertexCount = 4;
    square.indexCount = 6;
    square.vertices = std::vector<GfxVertex>(square.vertexCount);
    square.indices = std::vector<uint32_t>(square.indexCount);

    square.vertices[0].position = glm::vec3(-0.5f,  0.5f, 0.0f);
    square.vertices[1].position = glm::vec3( 0.5f,  0.5f, 0.0f);
    square.vertices[2].position = glm::vec3(-0.5f, -0.5f, 0.0f);
    square.vertices[3].position = glm::vec3( 0.5f, -0.5f, 0.0f);

    square.vertices[0].normal = glm::vec3(0.0f, 0.0f, 1.0f);
    square.vertices[1].normal = glm::vec3(0.0f, 0.0f, 1.0f);
    square.vertices[2].normal = glm::vec3(0.0f, 0.0f, 1.0f);
    square.vertices[3].normal = glm::vec3(0.0f, 0.0f, 1.0f);

    square.vertices[0].uv = glm::vec2(0.0f, 0.0f);
    square.vertices[1].uv = glm::vec2(1.0f, 0.0f);
    square.vertices[2].uv = glm::vec2(0.0f, 1.0f);
    square.vertices[3].uv = glm::vec2(1.0f, 1.0f);

    square.indices = {
        2, 1, 0,
        3, 1, 2
    };

    return square;
}

Shape plane()
{
    Shape square;
    square.vertexCount = 4;
    square.indexCount = 6;
    square.vertices = std::vector<GfxVertex>(square.vertexCount);
    square.indices = std::vector<uint32_t>(square.indexCount);

    square.vertices[0].position = glm::vec3(-2.5f, 0.0f, -2.5f);
    square.vertices[1].position = glm::vec3( 2.5f, 0.0f, -2.5f);
    square.vertices[2].position = glm::vec3(-2.5f, 0.0f,  2.5f);
    square.vertices[3].position = glm::vec3( 2.5f, 0.0f,  2.5f);

    square.vertices[0].normal = glm::vec3(0.0f, 1.0f, 0.0f);
    square.vertices[1].normal = glm::vec3(0.0f, 1.0f, 0.0f);
    square.vertices[2].normal = glm::vec3(0.0f, 1.0f, 0.0f);
    square.vertices[3].normal = glm::vec3(0.0f, 1.0f, 0.0f);

    square.vertices[0].uv = glm::vec2(0.0f, 0.0f);
    square.vertices[1].uv = glm::vec2(1.0f, 0.0f);
    square.vertices[2].uv = glm::vec2(0.0f, 1.0f);
    square.vertices[3].uv = glm::vec2(1.0f, 1.0f);

    square.indices = {
        2, 1, 0,
        3, 1, 2
    };

    return square;
}

Shape grid()
{
    int resolution = 200;
    float size = 10.0f;
    Shape grid;
    grid.vertexCount = (resolution + 1) * (resolution + 1);
    grid.indexCount = resolution * resolution * 6;
    grid.vertices.resize(grid.vertexCount);
    grid.indices.resize(grid.indexCount);

    float step = size / resolution;
    float offset = size / 2.0f;

    // Create vertices
    for (int i = 0; i <= resolution; ++i) {
        for (int j = 0; j <= resolution; ++j) {
            float x = j * step - offset;
            float z = i * step - offset;
            grid.vertices[i * (resolution + 1) + j].position = glm::vec3(x, 0.0f, z);
            grid.vertices[i * (resolution + 1) + j].normal = glm::vec3(0.0f, 1.0f, 0.0f);
            grid.vertices[i * (resolution + 1) + j].uv = glm::vec2(j / float(resolution), i / float(resolution));
        }
    }

    // Create indices for 2 triangles per square
    int index = 0;
    for (int i = 0; i < resolution; ++i) {
        for (int j = 0; j < resolution; ++j) {
            int topLeft = i * (resolution + 1) + j;
            int topRight = topLeft + 1;
            int bottomLeft = topLeft + (resolution + 1);
            int bottomRight = bottomLeft + 1;

            grid.indices[index++] = topLeft;
            grid.indices[index++] = bottomLeft;
            grid.indices[index++] = bottomRight;

            grid.indices[index++] = topLeft;
            grid.indices[index++] = bottomRight;
            grid.indices[index++] = topRight;
        }
    }

    return grid;
}

Shape cube()
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
    cube.vertices[0].uv = glm::vec2(0.0f, 0.0f);
    cube.vertices[1].uv = glm::vec2(1.0f, 0.0f);
    cube.vertices[2].uv = glm::vec2(0.0f, 1.0f);
    cube.vertices[3].uv = glm::vec2(1.0f, 1.0f);
    // Front face
    cube.vertices[4].uv = glm::vec2(0.0f, 0.0f);
    cube.vertices[5].uv = glm::vec2(1.0f, 0.0f);
    cube.vertices[6].uv = glm::vec2(0.0f, 1.0f);
    cube.vertices[7].uv = glm::vec2(1.0f, 1.0f);

    // Right face
    cube.vertices[8].uv = glm::vec2(0.0f, 0.0f);
    cube.vertices[9].uv = glm::vec2(1.0f, 0.0f);
    cube.vertices[10].uv = glm::vec2(0.0f, 1.0f);
    cube.vertices[11].uv = glm::vec2(1.0f, 1.0f);
    // Back face
    cube.vertices[12].uv = glm::vec2(0.0f, 0.0f);
    cube.vertices[13].uv = glm::vec2(1.0f, 0.0f);
    cube.vertices[14].uv = glm::vec2(0.0f, 1.0f);
    cube.vertices[15].uv = glm::vec2(1.0f, 1.0f);
    // Left face
    cube.vertices[16].uv = glm::vec2(0.0f, 0.0f);
    cube.vertices[17].uv = glm::vec2(1.0f, 0.0f);
    cube.vertices[18].uv = glm::vec2(0.0f, 1.0f);
    cube.vertices[19].uv = glm::vec2(1.0f, 1.0f);
    // Bottom face
    cube.vertices[20].uv = glm::vec2(0.0f, 0.0f);
    cube.vertices[21].uv = glm::vec2(1.0f, 0.0f);
    cube.vertices[22].uv = glm::vec2(0.0f, 1.0f);
    cube.vertices[23].uv = glm::vec2(1.0f, 1.0f);

    return cube;
}

Shape skybox()
{
    Shape skybox;
    skybox.vertexCount = 24;
    skybox.indexCount = 36;
    skybox.vertices = std::vector<GfxVertex>(skybox.vertexCount);
    skybox.indices = std::vector<uint32_t>(skybox.indexCount);

    skybox.indices = {
         0,  1,  2, // Top
         2,  1,  3,
         4,  5,  6, // Back
         6,  5,  7,
         8,  9, 10, // Right
        10,  9, 11,
        12, 13, 14, // Front
        14, 13, 15,
        16, 17, 18, // Left
        18, 17, 19,
        20, 21, 22, // Bottom
        22, 21, 23
    };

    skybox.vertices[0].position  = glm::vec3(-100.0f,  100.0f, -100.0f);
    skybox.vertices[1].position  = glm::vec3( 100.0f,  100.0f, -100.0f);
    skybox.vertices[2].position  = glm::vec3(-100.0f,  100.0f,  100.0f);
    skybox.vertices[3].position  = glm::vec3( 100.0f,  100.0f,  100.0f);
    skybox.vertices[4].position  = glm::vec3(-100.0f,  100.0f,  100.0f);
    skybox.vertices[5].position  = glm::vec3( 100.0f,  100.0f,  100.0f);
    skybox.vertices[6].position  = glm::vec3(-100.0f, -100.0f,  100.0f);
    skybox.vertices[7].position  = glm::vec3( 100.0f, -100.0f,  100.0f);
    skybox.vertices[8].position  = glm::vec3( 100.0f,  100.0f,  100.0f);
    skybox.vertices[9].position  = glm::vec3( 100.0f,  100.0f, -100.0f);
    skybox.vertices[10].position = glm::vec3( 100.0f, -100.0f,  100.0f);
    skybox.vertices[11].position = glm::vec3( 100.0f, -100.0f, -100.0f);
    skybox.vertices[12].position = glm::vec3( 100.0f,  100.0f, -100.0f);
    skybox.vertices[13].position = glm::vec3(-100.0f,  100.0f, -100.0f);
    skybox.vertices[14].position = glm::vec3( 100.0f, -100.0f, -100.0f);
    skybox.vertices[15].position = glm::vec3(-100.0f, -100.0f, -100.0f);
    skybox.vertices[16].position = glm::vec3(-100.0f,  100.0f, -100.0f);
    skybox.vertices[17].position = glm::vec3(-100.0f,  100.0f,  100.0f);
    skybox.vertices[18].position = glm::vec3(-100.0f, -100.0f, -100.0f);
    skybox.vertices[19].position = glm::vec3(-100.0f, -100.0f,  100.0f);
    skybox.vertices[20].position = glm::vec3(-100.0f, -100.0f,  100.0f);
    skybox.vertices[21].position = glm::vec3( 100.0f, -100.0f,  100.0f);
    skybox.vertices[22].position = glm::vec3(-100.0f, -100.0f, -100.0f);
    skybox.vertices[23].position = glm::vec3( 100.0f, -100.0f, -100.0f);

    skybox.vertices[0].normal  = glm::vec3( 0.0f,  -1.0f,  0.0f); //Top
    skybox.vertices[1].normal  = glm::vec3( 0.0f,  -1.0f,  0.0f);
    skybox.vertices[2].normal  = glm::vec3( 0.0f,  -1.0f,  0.0f); 
    skybox.vertices[3].normal  = glm::vec3( 0.0f,  -1.0f,  0.0f);
    skybox.vertices[4].normal  = glm::vec3( 0.0f,  0.0f,  -1.0f); //Front
    skybox.vertices[5].normal  = glm::vec3( 0.0f,  0.0f,  -1.0f);
    skybox.vertices[6].normal  = glm::vec3( 0.0f,  0.0f,  -1.0f);
    skybox.vertices[7].normal  = glm::vec3( 0.0f,  0.0f,  -1.0f);
    skybox.vertices[8].normal  = glm::vec3( -1.0f,  0.0f,  0.0f); //Right
    skybox.vertices[9].normal  = glm::vec3( -1.0f,  0.0f,  0.0f);
    skybox.vertices[10].normal = glm::vec3( -1.0f,  0.0f,  0.0f);
    skybox.vertices[11].normal = glm::vec3( -1.0f,  0.0f,  0.0f);
    skybox.vertices[12].normal = glm::vec3( 0.0f,  0.0f, 1.0f); //Back
    skybox.vertices[13].normal = glm::vec3( 0.0f,  0.0f, 1.0f);
    skybox.vertices[14].normal = glm::vec3( 0.0f,  0.0f, 1.0f);
    skybox.vertices[15].normal = glm::vec3( 0.0f,  0.0f, 1.0f);
    skybox.vertices[16].normal = glm::vec3(1.0f,  0.0f,  0.0f); //Left
    skybox.vertices[17].normal = glm::vec3(1.0f,  0.0f,  0.0f);
    skybox.vertices[18].normal = glm::vec3(1.0f,  0.0f,  0.0f);
    skybox.vertices[19].normal = glm::vec3(1.0f,  0.0f,  0.0f);
    skybox.vertices[20].normal = glm::vec3( 0.0f, 1.0f,  0.0f); //Bottom
    skybox.vertices[21].normal = glm::vec3( 0.0f, 1.0f,  0.0f);
    skybox.vertices[22].normal = glm::vec3( 0.0f, 1.0f,  0.0f);
    skybox.vertices[23].normal = glm::vec3( 0.0f, 1.0f,  0.0f);

    skybox.vertices[0].uv = glm::vec2(0.0f, 0.0f); //Top
    skybox.vertices[1].uv = glm::vec2(1.0f, 0.0f);
    skybox.vertices[2].uv = glm::vec2(0.0f, 1.0f);
    skybox.vertices[3].uv = glm::vec2(1.0f, 1.0f);
    skybox.vertices[4].uv = glm::vec2(0.0f, 0.0f); //Front
    skybox.vertices[5].uv = glm::vec2(1.0f, 0.0f);
    skybox.vertices[6].uv = glm::vec2(0.0f, 1.0f);
    skybox.vertices[7].uv = glm::vec2(1.0f, 1.0f);
    skybox.vertices[8].uv = glm::vec2(0.0f, 0.0f); //Right
    skybox.vertices[9].uv = glm::vec2(1.0f, 0.0f);
    skybox.vertices[10].uv = glm::vec2(0.0f, 1.0f);
    skybox.vertices[11].uv = glm::vec2(1.0f, 1.0f);
    skybox.vertices[12].uv = glm::vec2(0.0f, 0.0f); //Back
    skybox.vertices[13].uv = glm::vec2(1.0f, 0.0f);
    skybox.vertices[14].uv = glm::vec2(0.0f, 1.0f);
    skybox.vertices[15].uv = glm::vec2(1.0f, 1.0f);
    skybox.vertices[16].uv = glm::vec2(0.0f, 0.0f); //Left
    skybox.vertices[17].uv = glm::vec2(1.0f, 0.0f);
    skybox.vertices[18].uv = glm::vec2(0.0f, 1.0f);
    skybox.vertices[19].uv = glm::vec2(1.0f, 1.0f);
    skybox.vertices[20].uv = glm::vec2(0.0f, 0.0f); //Bottom
    skybox.vertices[21].uv = glm::vec2(1.0f, 0.0f);
    skybox.vertices[22].uv = glm::vec2(0.0f, 1.0f);
    skybox.vertices[23].uv = glm::vec2(1.0f, 1.0f);

    return skybox;
}

Shape sphere()
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

Shape triangle() {
    Shape triangle;
    triangle.vertexCount = 3;
    triangle.indexCount = 3;
    triangle.vertices = std::vector<GfxVertex>(triangle.vertexCount);
    triangle.indices = std::vector<uint32_t>(triangle.indexCount);

    triangle.vertices[0].position = glm::vec3(0.5f, -0.5f, 0.0f);
    triangle.vertices[1].position = glm::vec3(0.0f, 0.5f, 0.0f);
    triangle.vertices[2].position = glm::vec3(-0.5f, -0.5f, 0.0f);

    triangle.vertices[0].uv = glm::vec2(1.0f, 0.0f);
    triangle.vertices[1].uv = glm::vec2(0.5f, 1.0f);
    triangle.vertices[2].uv = glm::vec2(0.0f, 0.0f);

    triangle.vertices[0].normal = glm::vec3(0.0f, 0.0f, -1.0f);
    triangle.vertices[1].normal = glm::vec3(0.0f, 0.0f, -1.0f);
    triangle.vertices[2].normal = glm::vec3(0.0f, 0.0f, -1.0f);

    triangle.indices = {0, 1, 2};

    return triangle;
}

Shape pyramid() {
    Shape pyramid;
    pyramid.vertexCount = 16;
    pyramid.indexCount = 18;
    pyramid.vertices = std::vector<GfxVertex>(pyramid.vertexCount);
    pyramid.indices = std::vector<uint32_t>(pyramid.indexCount);

    pyramid.vertices[0].position = glm::vec3(0.5f, -0.5f, -0.5f); // Front
    pyramid.vertices[1].position = glm::vec3(0.0f, 0.5f, 0.0f);
    pyramid.vertices[2].position = glm::vec3(-0.5f, -0.5f, -0.5f);
    pyramid.vertices[3].position = glm::vec3(0.5f, -0.5f, 0.5f); // Back
    pyramid.vertices[4].position = glm::vec3(0.0f, 0.5f, 0.0f);
    pyramid.vertices[5].position = glm::vec3(-0.5f, -0.5f, 0.5f);
    pyramid.vertices[6].position = glm::vec3(-0.5f, -0.5f, -0.5f); // Left
    pyramid.vertices[7].position = glm::vec3(0.0f, 0.5f, 0.0f);
    pyramid.vertices[8].position = glm::vec3(-0.5f, -0.5f, 0.5f);
    pyramid.vertices[9].position = glm::vec3(0.5f, -0.5f, -0.5f); // Right
    pyramid.vertices[10].position = glm::vec3(0.0f, 0.5f, 0.0f);
    pyramid.vertices[11].position = glm::vec3(0.5f, -0.5f, 0.5f);
    pyramid.vertices[12].position = glm::vec3(0.5f, -0.5f, 0.5f); //Bottom
    pyramid.vertices[13].position = glm::vec3( 0.5f, -0.5f, -0.5f);
    pyramid.vertices[14].position = glm::vec3(-0.5f, -0.5f, -0.5f);
    pyramid.vertices[15].position = glm::vec3(-0.5f, -0.5f,  0.5f);

    pyramid.vertices[0].uv = glm::vec2(1.0f, 0.0f); // Front
    pyramid.vertices[1].uv = glm::vec2(0.5f, 1.0f);
    pyramid.vertices[2].uv = glm::vec2(0.0f, 0.0f);
    pyramid.vertices[3].uv = glm::vec2(1.0f, 0.0f); // Back
    pyramid.vertices[4].uv = glm::vec2(0.5f, 1.0f);
    pyramid.vertices[5].uv = glm::vec2(0.0f, 0.0f);
    pyramid.vertices[6].uv = glm::vec2(1.0f, 0.0f); // Left
    pyramid.vertices[7].uv = glm::vec2(0.5f, 1.0f);
    pyramid.vertices[8].uv = glm::vec2(0.0f, 0.0f);
    pyramid.vertices[9].uv = glm::vec2(1.0f, 0.0f); // Right
    pyramid.vertices[10].uv = glm::vec2(0.5f, 1.0f);
    pyramid.vertices[11].uv = glm::vec2(0.0f, 0.0f);
    pyramid.vertices[12].uv = glm::vec2(0.0f, 0.0f); // Bottom
    pyramid.vertices[13].uv = glm::vec2(1.0f, 0.0f);
    pyramid.vertices[14].uv = glm::vec2(0.0f, 1.0f);
    pyramid.vertices[15].uv = glm::vec2(1.0f, 1.0f);

    pyramid.vertices[0].normal = glm::vec3(0.0f, 0.0f, -1.0f); // Front
    pyramid.vertices[1].normal = glm::vec3(0.0f, 0.0f, -1.0f);
    pyramid.vertices[2].normal = glm::vec3(0.0f, 0.0f, -1.0f);
    pyramid.vertices[3].normal = glm::vec3(0.0f, 0.0f, 1.0f); // Back
    pyramid.vertices[4].normal = glm::vec3(0.0f, 0.0f, 1.0f);
    pyramid.vertices[5].normal = glm::vec3(0.0f, 0.0f, 1.0f);
    pyramid.vertices[6].normal = glm::vec3(-1.0f, 0.0f, 0.0f); // Left
    pyramid.vertices[7].normal = glm::vec3(-1.0f, 0.0f, 0.0f);
    pyramid.vertices[8].normal = glm::vec3(-1.0f, 0.0f, 0.0f);
    pyramid.vertices[9].normal = glm::vec3(1.0f, 0.0f, 0.0f);  // Right
    pyramid.vertices[10].normal = glm::vec3(1.0f, 0.0f, 0.0f);
    pyramid.vertices[11].normal = glm::vec3(1.0f, 0.0f, 0.0f);
    pyramid.vertices[12].normal = glm::vec3(0.0f, -1.0f, 0.0f);  // Bototm
    pyramid.vertices[13].normal = glm::vec3(0.0f, -1.0f, 0.0f);
    pyramid.vertices[14].normal = glm::vec3(0.0f, -1.0f, 0.0f);
    pyramid.vertices[15].normal = glm::vec3(0.0f, -1.0f, 0.0f);

    pyramid.indices = {
        2, 1, 0,
        5, 3, 4,
        8, 7, 6,
        11, 9, 10,
        14, 13, 15,
        15, 13, 12
    };

    return pyramid;
}

Shape Shapes::getShape(Types type)
{
    Shape shape;
	switch (type)
	{
    case SQUARE:
        shape = square();
        break;
    case PLANE:
        shape = plane();
        break;
    case GRID:
        shape = grid();
        break;
	case CUBE:
		shape = cube();
		break;
	case SKYBOX:
		shape = skybox();
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

    return shape;
}
