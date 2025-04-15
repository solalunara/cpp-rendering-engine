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