// This file is an adapted version of a file that originated in 2022 by the same author
// as part of a project that received periodic updates through 2024
// the changes in this version are
//      - un-deletion of move constructor and declaration of move operator
//      - corrected handling of virtual render() function
//      - removal of references to smallest_inter_point_distance and vertex point cache
//      - replace texture with globaltexture
//      - replace raw pointers for transform and window with smart pointers
// to see the original, go to https://github.com/solalunara/phys/blob/main/src/render/mesh.h

#ifndef MESH_H
#define MESH_H

#pragma once

#include "transform.h"
#include "renderable.h"
#include "GlobalTexture.h"
#include <memory>
using glm::vec2;
using glm::vec3;
using std::unique_ptr;

struct Texture;
struct Shader;
struct Window;
struct GlobalTexture;
struct Word;

struct Mesh :
    public Renderable
{
    Mesh( vec2 mins, vec2 maxs, GlobalTexture texture, std::unique_ptr<Transform> &&transform, const std::shared_ptr<Window> &container );
    Mesh( float *verts, unsigned long long verts_len, unsigned int *inds, unsigned long long inds_len, GlobalTexture texture, std::unique_ptr<Transform> &&transform, const std::shared_ptr<Window> &container );
    virtual ~Mesh();

    Mesh( const Mesh & ) = delete;
    Mesh &operator =( const Mesh & ) = delete;

    Mesh( Mesh &&other );
    Mesh &operator =( Mesh &&other );

    virtual void render() const;


    virtual inline bool is_text() const { return false; }
    virtual inline bool is_mesh() const override { return true; }

    virtual vec3 get_normal() const override;
    virtual float get_plane_dist() const override;
    virtual vector<vec3> get_vertices( bool local = false );

    const GlobalTexture &texture = m_texture;

protected:
    unsigned int m_VBO;
    unsigned int m_VAO;
    unsigned int m_EBO;

    float *m_verts;
    unsigned long long m_verts_len;

    unsigned int *m_inds;
    unsigned long long m_inds_len;

    GlobalTexture m_texture;

    vec3 m_norm;
};

struct CharacterMesh : 
    public Mesh
{
    CharacterMesh( vec2 mins, vec2 maxs, GlobalTexture texture, std::unique_ptr<Transform> &&transform, const std::shared_ptr<Window> &container, vec3 Color, char c, bool UI ) :
        Mesh( mins, maxs, texture, std::move( transform ), container ), Color( Color ), c( c )
    {
        //textures are naturally upside-down, flip them on the local x axis
        this->m_transform->set_local_rotation( glm::angleAxis( glm::radians( 180.f ), vec3( 1, 0, 0 ) ) * transform->get_local_rotation() );
    }

    virtual void render() const override;

    virtual bool IsText() { return true; }

    vec3 Color;
    char c;
};

#endif