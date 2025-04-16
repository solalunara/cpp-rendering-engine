// This file is an adapted version of a file that originated in 2022 by the same author
// as part of a project that received periodic updates through 2024
// the file in that project was a derivative of a file originally written by Joey de Vries made avaliable via learnopengl.com
// where much of the original logic for interfacing with opengl was written
// the changes in this version are
//      - move constructor and assignment operator declaration and definition
//      - reworking public/private split
//      - storing string instead of const char * for memory safety
// to see the original, go to https://github.com/solalunara/phys/blob/main/src/render/texture.h
// to see the file by Joey de Vries, go to https://github.com/JoeyDeVries/LearnOpenGL/blob/master/includes/learnopengl/mesh.h

#ifndef TEXTURE_H
#define TEXTURE_H

#include <cstring>
#include <memory>
#include <string>
using std::string;
using std::shared_ptr;

#pragma once

struct GLFWwindow;
typedef GLFWwindow *WindowID;

struct Window;
struct Texture
{
    // this is called from inside the window constructor and thus cannot take a smart pointer handle
    Texture( const char *path, Window &container );

    Texture( const unsigned char *buffer, const char *name, unsigned int width, unsigned int rows, const shared_ptr<Window> &container );
    ~Texture();

    Texture( const Texture & ) = delete;
    Texture &operator =( const Texture & ) = delete;

    Texture( Texture &&other ) : m_id( other.m_id ), m_path( std::move( other.m_path ) )
    {
        other.m_id = 0;
        other.m_container_id = 0;
    }
    Texture &operator =( Texture &&other )
    {
        if ( &other == this )
            return *this;

        m_id = other.m_id;
        m_path = std::move( other.m_path );
        other.m_id = 0;
        other.m_container_id = 0;

        return *this;
    }

    const unsigned int &id = m_id;
    std::string get_path() const { return m_path; }

private:
    WindowID m_container_id;
    string m_path;
    unsigned int m_id;
};

#endif