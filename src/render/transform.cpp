#include "transform.h"
#include "renderable.h"

Transform::Transform( Transform &&other )
{
    m_pos = other.m_pos;
    m_rot = other.m_rot;
    m_scl = other.m_scl;
    m_owning_object = other.m_owning_object;
    other.m_owning_object = NULL;
}
Transform &Transform::operator =( Transform &&other )
{
    if ( &other == this )
        return *this;

    m_pos = other.m_pos;
    m_rot = other.m_rot;
    m_scl = other.m_scl;
    m_owning_object = other.m_owning_object;
    other.m_owning_object = NULL;

    return *this;
}
Transform::Transform( vec3 pos, quat rot, vec3 scl ) : 
    m_pos( pos ), m_rot( rot ), m_scl( scl ), m_owning_object( NULL )
{
}

mat4 Transform::get_matrix() const
{
    return ( ( m_owning_object && m_owning_object->parent ) ? m_owning_object->parent->get_transform_matrix() : glm::identity<mat4>() ) * glm::translate( mat4( 1 ), m_pos ) * glm::mat4_cast( m_rot ) * glm::scale( mat4( 1 ), m_scl );
}
mat4 Transform::get_inverse_matrix() const
{
    return glm::inverse( get_matrix() );
}
vec3 Transform::get_abs_origin() const
{
    vec3 scl;
    quat rot;
    vec3 pos;
    vec3 skew;
    vec4 persp;
    glm::decompose( get_matrix(), scl, rot, pos, skew, persp );
    return pos;
}
quat Transform::get_abs_rotation() const
{
    vec3 scl;
    quat rot;
    vec3 pos;
    vec3 skew;
    vec4 persp;
    glm::decompose( get_matrix(), scl, rot, pos, skew, persp );
    return rot;
}
vec3 Transform::get_local_origin() const
{
    return m_pos;
}
quat Transform::get_local_rotation() const
{
    return m_rot;
}
void Transform::set_abs_origin(vec3 pt)
{
    if ( !m_owning_object || !m_owning_object->parent )
        m_pos = pt;
    else
        m_pos = m_owning_object->parent->get_inverse_transform_matrix() * vec4( m_pos, 1 );
}
void Transform::set_abs_rotation( quat rt )
{
    if ( !m_owning_object || !m_owning_object->parent )
        m_rot = rt;
    else
    {
        vec3 scl;
        quat rot_parent;
        vec3 pos;
        vec3 skew;
        vec4 persp;
        glm::decompose( m_owning_object->parent->get_transform_matrix(), scl, rot_parent, pos, skew, persp );
        m_rot = glm::inverse( rot_parent ) * rt;
    }
}

void Transform::set_local_origin( vec3 pt )
{
    m_pos = pt;
}

void Transform::set_local_rotation( quat rt )
{
    m_rot = rt;
}
