// This file is an adapted version of a file that originated in 2022 by the same author
// as part of a project that received periodic updates through 2024
// the changes in this version are
//      - un-deleting move constructor and declaring move assignment operator
//      - refactoring pointers to smart pointers and reworking the associated logic
//      - reworked public/protected data split
//      - claim_all_children(), a completely new helper function for setting the parent of children from derived classes
//      - const handling
// to see the original, go to https://github.com/solalunara/phys/blob/main/src/render/renderable.h

#ifndef RENDERABLE_H
#define RENDERABLE_H

struct Window;
struct Mesh;
struct Transform;

#include <vector>
#include <memory>
using std::shared_ptr;
using std::unique_ptr;
using std::weak_ptr;
using std::vector;

#include <glm/glm.hpp>
using glm::vec3;
using glm::mat4;

#include <math.h>

struct Renderable
{
    Renderable( const Renderable & ) = delete;
    Renderable &operator =( const Renderable & ) = delete;

    virtual ~Renderable();

    virtual vec3 get_normal() const = 0;
    virtual float get_plane_dist() const = 0;
    virtual vector<vec3> get_vertices( bool local = false ) const;

    virtual void render() const;
    virtual inline bool is_mesh() const { return false; }
    vector<vec3> get_normals() const;
    vector<float> get_plane_dists() const;

    const Renderable * const &parent = m_parent;
    const mat4 get_transform_matrix() const;
    const mat4 get_inverse_transform_matrix() const;

    void add_child( unique_ptr<Renderable> &&child );
protected:
    //abstract class, all constructors & operators should only be accessible from derived classes
    Renderable( const shared_ptr<Window> &container, std::unique_ptr<Transform> &&transform, vector<unique_ptr<Renderable>> &&Elements );
    Renderable( Renderable &&other );
    Renderable &operator =( Renderable &&other );

    void claim_all_children();

    vector<unique_ptr<Renderable>> m_elements;
    unique_ptr<Transform> m_transform;

    //parent owns child, so this pointer is always valid or null
    Renderable *m_parent;

    //window owns root renderable for this renderable, which owns this renderable -
    //this object cannot live past the lifetime of the window
    Window *m_container;
};

#endif // RENDERABLE_H