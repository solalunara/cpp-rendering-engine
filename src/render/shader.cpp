#include "shader.h"

#include "../../assets/shaders/fragment.h"
#include "../../assets/shaders/vertex.h"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

unsigned int CreateShader( ShaderType type, WindowID window )
{
    glfwMakeContextCurrent( window );
    unsigned int id;
    switch ( type )
    {
        case ShaderType::VERTEX:
        id = glCreateShader( GL_VERTEX_SHADER );
        glShaderSource( id, 1, &g_pszVertexShaderSource, NULL );
        break;

        case ShaderType::FRAGMENT:
        id = glCreateShader( GL_FRAGMENT_SHADER );
        glShaderSource( id, 1, &g_pszFragShaderSource, NULL );
        break;
    }
    glCompileShader( id );

    int state;
    glGetShaderiv( id, GL_COMPILE_STATUS, &state );
    if ( !state )
    {
        char info[ 2048 ] { 0 };
        glGetShaderInfoLog( id, 2048, NULL, info );
        const char *name;
        switch ( type )
        {
            case ShaderType::VERTEX:
                name = "vertex";
                break;
            case ShaderType::FRAGMENT:
                name = "fragment";
                break;
            default:
                name = "unknown";
                break;
        };
        printf( "Couldn't compile %s shader - %s\n", name, info );
    }

    return id;
}


Shader::Shader() :
    m_id( -1 )
{
}
Shader::Shader( Shader &&other ) : 
    m_id( other.m_id ),
    m_owning_window( other.m_owning_window )
{
    other.m_id = -1;
    other.m_owning_window = 0;
}
Shader::~Shader()
{
    glfwMakeContextCurrent( m_owning_window );
    if ( id > -1 )
        glDeleteProgram( id );
}

void Shader::operator =( Shader &&other )
{
    m_id = other.m_id;
    m_owning_window = other.m_owning_window;
    other.m_id = -1;
    other.m_owning_window = 0;
}

void Shader::InitShader( WindowID window )
{
    m_owning_window = window;
    glfwMakeContextCurrent( window );
    m_id = glCreateProgram();

    unsigned int vert = CreateShader( ShaderType::VERTEX, window );
    glAttachShader( id, vert );
    
    unsigned int frag = CreateShader( ShaderType::FRAGMENT, window );
    glAttachShader( id, frag );

    glLinkProgram( id );

    int state;
    glGetProgramiv( id, GL_LINK_STATUS, &state );
    if ( !state )
    {
        char info[ 2048 ] { 0 };
        glGetProgramInfoLog( id, 2048, NULL, info );
        printf( "Couldn't link shader program - %s\n", info );
    }

    glDeleteShader( vert );
    glDeleteShader( frag );
}

void Shader::Use()
{
    glfwMakeContextCurrent( m_owning_window );
    glUseProgram( id );
}

void Shader::SetShaderValue( const char *name, bool val )
{
    Use();
    glUniform1i( glGetUniformLocation( id, name ), val );
}
void Shader::SetShaderValue( const char *name, int val )
{
    Use();
    glUniform1i( glGetUniformLocation( id, name ), val );
}
void Shader::SetShaderValue( const char *name, float val )
{
    Use();
    glUniform1f( glGetUniformLocation( id, name ), val );
}
void Shader::SetShaderValue( const char *name, glm::mat4 val )
{
    Use();
    glUniformMatrix4fv( glGetUniformLocation( id, name ), 1, false, &val[ 0 ][ 0 ] );
}