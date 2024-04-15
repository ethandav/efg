#pragma once
#ifndef SHAPES_H
#define SHAPES_H

#include <vector>
#include <glm/ext/matrix_transform.hpp>
#include <gfx_scene.h>

struct Vertex
{
    glm::vec3 position  = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 normal    = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec2 uv        = glm::vec2(0.0f, 0.0f);
};

struct Shape
{
    int                     vertexCount     = 0;
    int                     indexCount      = 0;
    std::vector<GfxVertex>  vertices        = {};
    std::vector<uint32_t>   indices         = {};
};

namespace Shapes
{
    enum Types
    {
        SQUARE,
        CUBE,
        SKYBOX,
        SPHERE,
        TRIANGLE,
        PYRAMID
    };

    Shape getShape(Types type);
}

#endif
