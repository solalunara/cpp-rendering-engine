#ifndef SHADER_H
#define SHADER_H

#pragma once

#include <glm/glm.hpp>

//only vertex and fragment shader implemented thus far
enum class ShaderType : bool { VERTEX, FRAGMENT };

struct GLFWwindow;
typedef GLFWwindow *WindowID;

//Encapsulation of shader pipeline for rendering
struct Shader
{
    Shader();
    ~Shader();

    //fundamentally uncopyable objects - either move or make a new one
    //destruction of a copy of a shader frees memory in opengl that is needed for the original
    Shader( const Shader & ) = delete;
    Shader &operator =( const Shader & ) = delete;

    void operator =( Shader &&other );
    Shader( Shader &&other );

    void SetShaderValue( const char *name, bool val );
    void SetShaderValue( const char *name, int val );
    void SetShaderValue( const char *name, float val );
    void SetShaderValue( const char *name, glm::mat4 val );

    void Use();
    void InitShader( WindowID window );

    const unsigned int &id = m_id;

private:
    unsigned int m_id;
    WindowID m_owning_window;
};

#endif