// This file is an adapted version of a file that originated in 2022 by the same author
// as part of a project that received periodic updates through 2024
// the file in that project was a derivative of a file originally written by Joey de Vries made avaliable via learnopengl.com
// where much of the original logic for interfacing with opengl was written
// the changes in this version are
//      - move constructor and assignment operator declaration
//      - tracking of windowID
//      - addition of InitShader(), to be called when the program is ready to init data
// to see the original, go to https://github.com/solalunara/phys/blob/main/src/render/shader.h
// to see the file by Joey de Vries, go to https://github.com/JoeyDeVries/LearnOpenGL/blob/master/includes/learnopengl/shader.h

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