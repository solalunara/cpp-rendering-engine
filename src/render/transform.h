#ifndef TRANSFORM_H
#define TRANSFORM_H

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::quat;
using glm::mat4;

#include <vector>
using std::vector;

struct Renderable;

struct Transform
{
    // if a transform is being copied then said copy exists outside the scope of the owning renderable
    // and the validity of the owning object pointer cannot be verified
    Transform( const Transform & ) = delete;
    Transform &operator =( const Transform & ) = delete;

    Transform( Transform &&other );
    Transform &operator =( Transform &&other );

    Transform( vec3 pos, quat rot, vec3 scl );

    ~Transform() {}

    mat4 get_matrix() const;
    mat4 get_inverse_matrix() const;

    vec3 get_abs_origin() const;
    quat get_abs_rotation() const;

    vec3 get_local_origin() const;
    quat get_local_rotation() const;

    void set_abs_origin( vec3 pt );
    void set_abs_rotation( quat rt );

    void set_local_origin( vec3 pt );
    void set_local_rotation( quat rt );

    vec3 local_to_world_direction( vec3 v ) const { return get_matrix() * vec4( v, 0 ); }
    vec3 local_to_world_point( vec3 p ) const { return get_matrix() * vec4( p, 1 ); }
    vec2 local_to_world_point( vec2 p ) const { return get_matrix() * vec4( p, 0, 1 ); }
    vec3 world_to_local_direction( vec3 v ) const { return get_inverse_matrix() * vec4( v, 0 ); }
    vec3 world_to_local_point( vec3 p ) const { return get_inverse_matrix() * vec4( p, 1 ); }
    vec2 world_to_local_point( vec2 p ) const { return get_inverse_matrix() * vec4( p, 0, 1 ); }

private:
    vec3 m_pos;
    quat m_rot;
    vec3 m_scl;
    Renderable *m_owning_object;

    //renderable ctor needs to set owning object, but should be the only thing able to do so
    friend class Renderable; 
};

#endif