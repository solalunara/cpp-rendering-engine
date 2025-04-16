// This file is a heavily adapted version of a file that originated in 2022 by the same author
// as part of a project that received periodic updates through 2024
// the changes in this version are
//      - default constructor declaration
//      - move/copy constructor declarations
//      - move/copy assignment operator declarations
//      - const char * to string for memory safety
//      - everything relating to RawTextureData
//      - removal of the find_local_texture() function
// to see the original, go to https://github.com/solalunara/phys/blob/main/src/render/GlobalTexture.h

#ifndef GLOBALTEXTURE_H
#define GLOBALTEXTURE_H

#pragma once

#include <vector>
#include <string>
#include <memory>
#include <array>
using std::vector;
using std::string;
using std::basic_string;
using std::shared_ptr;
using std::unique_ptr;
using std::make_shared;

struct Texture;
struct Window;
typedef unsigned long long TextureID;

struct RawTextureData
{
    RawTextureData( basic_string<unsigned char> buffer, string name, unsigned int width, unsigned int rows ) :
        m_buffer( buffer ), m_name( name ), m_width( width ), m_rows( rows )
    {
    }
    RawTextureData( const RawTextureData &other ) :
        m_buffer( other.m_buffer ), m_name( other.m_name ), m_width( other.m_width ), m_rows( other.m_rows )
    {
    }
    RawTextureData( RawTextureData &&other ) :
        m_buffer( std::move( other.m_buffer ) ), m_name( std::move( other.m_name ) ), m_width( other.m_width ), m_rows( other.m_rows )
    {
    }
    RawTextureData &operator =( const RawTextureData &other )
    {
        if ( &other == this )
            return *this;

        m_buffer = other.m_buffer;
        m_name = other.m_name;
        m_width = other.m_width;
        m_rows = other.m_rows;

        return *this;
    }
    RawTextureData &operator =( RawTextureData &&other )
    {
        if ( &other == this )
            return *this;

        m_buffer = std::move( other.m_buffer );
        m_name = std::move( other.m_name );
        m_width = other.m_width;
        m_rows = other.m_rows;

        return *this;
    }

    std::basic_string<unsigned char> m_buffer;
    string m_name;
    unsigned int m_width;
    unsigned int m_rows;
};

class GlobalTexture
{
public:
    GlobalTexture();
    GlobalTexture( const char *path );
    GlobalTexture( std::unique_ptr<RawTextureData> &&data );
    
    GlobalTexture( const GlobalTexture &other );
    GlobalTexture( GlobalTexture &&other );
    ~GlobalTexture();

    GlobalTexture &operator =( const GlobalTexture &other );
    GlobalTexture &operator =( GlobalTexture &&other );

    string get_path() const { return m_path; }

private:

    string m_path;
    unique_ptr<RawTextureData> m_data;
};

#endif
