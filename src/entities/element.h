#ifndef ELEMENT_H
#define ELEMENT_H

#pragma once

#include "render/transform.h"
#include "render/window.h"
#include "render/renderable.h"
#include <vector>
#include <glm/glm.hpp>
using std::vector;
using glm::vec3;

struct Mesh;
struct Element;

// theoretical base class for "objects" in the program as opposed to merely renderable "things"
// e.g. if physics simulation were to be added at some later date
struct Element :
    public Renderable
{
    Element( const Element & ) = delete;
    Element &operator =( const Element & ) = delete;
    virtual ~Element();

    virtual vec3 get_normal() const { return glm::zero<vec3>(); }
    virtual float get_plane_dist() const { return 0; }

    virtual void render() const;
    virtual inline bool is_mesh() const { return false; };

protected:
    Element( const std::shared_ptr<Window> &container, std::unique_ptr<Transform> &&transform, vector<std::unique_ptr<Renderable>> &&Elements );
    Element( Element &&other );
    Element &operator =( Element &&other );

};

#endif
