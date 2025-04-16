// This file is an adapted version of a file that originated in 2022 by the same author
// as part of a project that received periodic updates through 2024
// the changes in this version are
//      - un-deleting move constructor and declaring move assignment operator
//      - refactoring pointers to smart pointers and reworking the associated logic
//      - const handling
//      - removing references to physics
// to see the original, go to https://github.com/solalunara/phys/blob/main/src/entities/element.h

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
