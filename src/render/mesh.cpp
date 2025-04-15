#include "mesh.h"

#include "texture.h"
#include "shader.h"
#include "GlobalTexture.h"
#include "window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <string.h>
#include <string>
#include <stdexcept>


Mesh::Mesh( float *verts, unsigned long long verts_len, unsigned int *inds, unsigned long long inds_len, GlobalTexture texture, std::unique_ptr<Transform> &&transform, const std::shared_ptr<Window> &container ) : 
    m_verts_len( verts_len ), m_inds_len( inds_len ), m_texture( texture ), Renderable( container, std::move( transform ), std::move( vector<std::unique_ptr<Renderable>>() ) )
{
    if ( !container )
        throw std::invalid_argument( "Attempted to make mesh with no container!\n" );

    glfwMakeContextCurrent( container->ID );
    this->m_verts = new float[ verts_len ];
    memcpy( this->m_verts, verts, verts_len * sizeof( float ) );

    this->m_inds = new unsigned int[ inds_len ];
    memcpy( this->m_inds, inds, inds_len * sizeof( unsigned int ) );

    glGenVertexArrays( 1, &m_VAO );
    glGenBuffers( 1, &m_VBO );
    glGenBuffers( 1, &m_EBO );

    glBindVertexArray( m_VAO );
    
    glBindBuffer( GL_ARRAY_BUFFER, m_VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * verts_len, verts, GL_STATIC_DRAW );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_EBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( unsigned int ) * inds_len, inds, GL_STATIC_DRAW );

    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), (void *)0 );
    glEnableVertexAttribArray( 0 );  
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), (void *)( 3 * sizeof( float ) ) );
    glEnableVertexAttribArray( 1 );

    glBindVertexArray( 0 );


    vector<vec3> __verts_pts;
    __verts_pts.reserve( m_verts_len / 5 );
    for ( int i = 0; i < m_verts_len / 5; ++i )
        __verts_pts.push_back( vec3( m_verts[ i*5 + 0 ], m_verts[ i*5 + 1 ], m_verts[ i*5 + 2 ] ) );
    __verts_pts.shrink_to_fit();

    if ( verts_len >= 3*5 )
    {
        vec3 a = __verts_pts[ 1 ] - __verts_pts[ 0 ];
        vec3 b = __verts_pts[ 2 ] - __verts_pts[ 1 ];
        m_norm = glm::normalize( glm::cross( a, b ) );
        if ( verts_len > 3*5 )
        {
            float dist = glm::dot( a, m_norm );
            for ( int i = 4; i < verts_len / 5; ++i )
                if ( std::abs( glm::dot( __verts_pts[ i ], m_norm ) - dist ) > 1e-2f )
                    throw std::invalid_argument( "Mesh does not lie on a plane!" );
        }
    }
}

Mesh::Mesh( glm::vec2 mins, glm::vec2 maxs, GlobalTexture texture, std::unique_ptr<Transform> &&transform, const std::shared_ptr<Window> &container ) :
    m_texture( texture ), Renderable( container, std::move( transform ), std::move( vector<std::unique_ptr<Renderable>>() ) )
{
    this->m_verts = new float[] {
        maxs.x, maxs.y, .0f,        1.0f, 1.0f,
        maxs.x, mins.y, .0f,        1.0f, 0.0f,
        mins.x, mins.y, .0f,        0.0f, 0.0f,
        mins.x, maxs.y, .0f,        0.0f, 1.0f,
    };
    this->m_verts_len = 20;
    this->m_inds = new unsigned int[] {
        0, 1, 3,
        1, 2, 3
    };
    this->m_inds_len = 6;

    if ( !container )
        throw std::invalid_argument( "Attempted to make mesh with no container!\n" );

    glfwMakeContextCurrent( container->ID );

    glGenVertexArrays( 1, &m_VAO );
    glGenBuffers( 1, &m_VBO );
    glGenBuffers( 1, &m_EBO );

    glBindVertexArray( m_VAO );
    
    glBindBuffer( GL_ARRAY_BUFFER, m_VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * m_verts_len, m_verts, GL_STATIC_DRAW );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_EBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( unsigned int ) * m_inds_len, m_inds, GL_STATIC_DRAW );

    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), (void *)0 );
    glEnableVertexAttribArray( 0 );  
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), (void *)( 3 * sizeof( float ) ) );
    glEnableVertexAttribArray( 1 );

    glBindVertexArray( 0 );

    vector<vec3> __verts_pts;
    __verts_pts.reserve( m_verts_len / 5 );
    for ( int i = 0; i < m_verts_len / 5; ++i )
        __verts_pts.push_back( vec3( m_verts[ i*5 + 0 ], m_verts[ i*5 + 1 ], m_verts[ i*5 + 2 ] ) );
    __verts_pts.shrink_to_fit();


    if ( m_verts_len >= 3*5 )
    {
        vec3 a = __verts_pts[ 1 ] - __verts_pts[ 0 ];
        vec3 b = __verts_pts[ 2 ] - __verts_pts[ 1 ];
        m_norm = glm::normalize( glm::cross( a, b ) );
        if ( m_verts_len > 3*5 )
        {
            float dist = glm::dot( a, m_norm );
            for ( int i = 4; i < m_verts_len / 5; ++i )
                if ( std::abs( glm::dot( __verts_pts[ i ], m_norm ) - dist ) > 1e-2f )
                    throw std::invalid_argument( "Mesh does not lie on a plane!" );
        }
    }
}

Mesh::~Mesh()
{
    glDeleteVertexArrays( 1, &m_VAO );
    glDeleteBuffers( 1, &m_VBO );
    glDeleteBuffers( 1, &m_EBO );
    m_VAO = m_VBO = m_EBO = 0;
    delete[] m_verts;
    delete[] m_inds;
}

Mesh::Mesh( Mesh &&other ) :
    Renderable( std::move( other ) ), m_VBO( other.m_VBO ), m_VAO( other.m_VAO ), m_EBO( other.m_EBO ),
    m_verts( other.m_verts ), m_verts_len( other.m_verts_len ), m_inds( other.m_inds ), m_inds_len( other.m_inds_len ),
    m_texture( other.m_texture )
{
    other.m_VBO = 0;
    other.m_VAO = 0;
    other.m_EBO = 0;
    other.m_verts = NULL;
    other.m_verts_len = 0;
    other.m_inds = NULL;
    other.m_inds_len = 0;
}

Mesh &Mesh::operator=( Mesh &&other )
{
    if ( &other == this )
        return other;

    Renderable::operator=( std::move( other ) );
    m_VBO = other.m_VBO;
    m_VAO = other.m_VAO; 
    m_EBO = other.m_EBO;
    m_verts = other.m_verts;
    m_verts_len = other.m_verts_len;
    m_inds = other.m_inds;
    m_inds_len = other.m_inds_len;

    other.m_VBO = 0;
    other.m_VAO = 0;
    other.m_EBO = 0;
    other.m_verts = NULL;
    other.m_verts_len = 0;
    other.m_inds = NULL;
    other.m_inds_len = 0;

    return *this;
}

void Mesh::render() const
{
    std::shared_ptr<Window> container = m_container.lock();
    if ( !container )
    {
        printf( "Error - cannot render mesh, window was deleted" );
        return;
    }
    glfwMakeContextCurrent( container->ID );
    container->get_shader().SetShaderValue( "Text", is_text() );
    container->get_shader().SetShaderValue( "Transform", m_transform->get_matrix() );
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, container->get_texture_opengl_id( texture ) );
    glBindVertexArray( m_VAO );
    glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
}

vec3 Mesh::get_normal() const
{
    vec3 norm = m_transform->local_to_world_direction( m_norm );
    return glm::round( norm * 1e4f ) / 1e4f;
}

float Mesh::get_plane_dist() const
{
    return glm::dot( m_transform->get_abs_origin(), get_normal() );
}

//TODO: this could be improved - as of now is simply the simplest method b/c copy paste from other parts of the code
vector<vec3> Mesh::get_vertices( bool local )
{
    vector<vec3> result;
    result.reserve( m_verts_len / 5 );
    for ( int i = 0; i < m_verts_len / 5; ++i )
        result.push_back( vec3( m_verts[ i*5 + 0 ], m_verts[ i*5 + 1 ], m_verts[ i*5 + 2 ] ) );
    result.shrink_to_fit();

    vector<vec3> __verts_pts;
    __verts_pts.reserve( m_verts_len / 5 );
    for ( int i = 0; i < m_verts_len / 5; ++i )
        __verts_pts.push_back( vec3( m_verts[ i*5 + 0 ], m_verts[ i*5 + 1 ], m_verts[ i*5 + 2 ] ) );
    __verts_pts.shrink_to_fit();

    if ( !local )
        for ( int i = 0; i < result.size(); ++i )
            result[ i ] = m_transform->local_to_world_point( __verts_pts[ i ] );
    return result;
}

void CharacterMesh::render() const
{
    if ( auto container = m_container.lock() )
    {
        container->get_shader().SetShaderValue( "TextColorX", Color.x );
        container->get_shader().SetShaderValue( "TextColorY", Color.y );
        container->get_shader().SetShaderValue( "TextColorZ", Color.z );
    }
    else
        printf( "Error - CharacterMesh has outlived its window" );
    Mesh::render();
}
