#pragma once
#ifndef SHAPES_H
#define SHAPES_H

#include <vector>
#include <glm/ext/matrix_transform.hpp>
#include <gfx_scene.h>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

struct Shape
{
    std::vector<GfxVertex> vertices;
    std::vector<uint32_t> indices;
    int vertexCount = 0;
    int indexCount = 0;
};

namespace Shapes
{
    enum Types
    {
        CUBE,
        SPHERE
    };

    void scale(Shape& shape, float scaleFactor);
    Shape cube();
    Shape sphere();
}

#endif
