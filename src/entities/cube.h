#ifndef ENTITY_H
#define ENTITY_H

#pragma once

#include "render/mesh.h"
#include "element.h"
#include <vector>
#include <glm/glm.hpp>
using std::vector;
using glm::vec2;
using glm::vec3;
using glm::quat;

struct Window;
struct Text;
struct GlobalTexture;

enum class Side : char
{
    NONE = 0,
    front = 1 << 0,
    back = 1 << 1,
    left = 1 << 2,
    right = 1 << 3,
    top = 1 << 4,
    bottom = 1 << 5,
};

struct Cube :
    public Element
{
    Cube( vec3 mins, vec3 maxs, std::unique_ptr<Transform> &&transform, GlobalTexture texture, const std::shared_ptr<Window> &container );
    Cube( vec3 mins, vec3 maxs, std::unique_ptr<Transform> &&transform, vector<GlobalTexture> textures, const std::shared_ptr<Window> &container );

    GlobalTexture front_tex;
    GlobalTexture back_tex;
    GlobalTexture left_tex;
    GlobalTexture right_tex;
    GlobalTexture top_tex;
    GlobalTexture bottom_tex;

private:
    vec3 mins;
    vec3 maxs;
};

#endif