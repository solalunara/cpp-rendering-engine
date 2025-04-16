// This file is a heavily adapted version of a file that originated in 2022 by the same author
// as part of a project that received periodic updates through 2024
// the changes in this version are
//      - default constructor definition
//      - move/copy constructor definitions
//      - move/copy assignment operator definitions
//      - const char * to string for memory safety
//      - everything relating to RawTextureData
//      - removal of the find_local_texture() function
// to see the original, go to https://github.com/solalunara/phys/blob/main/src/render/GlobalTexture.cpp

#include "GlobalTexture.h"
#include "window.h"
#include "texture.h"
#include <string.h>
using std::unique_ptr;
using std::make_unique;

GlobalTexture::GlobalTexture() :
    m_path( "" )
{
    m_data = NULL;
}

GlobalTexture::GlobalTexture( const char *path ) :
    m_path(path)
{
    m_data = NULL;
}
GlobalTexture::GlobalTexture( std::unique_ptr<RawTextureData> &&data ) :
    m_path( data->m_name ), m_data( std::move( data ) )
{
}

GlobalTexture::GlobalTexture( const GlobalTexture &other )
{
    m_path = other.m_path;
    m_data = other.m_data ? make_unique<RawTextureData>( *other.m_data.get() ) : 0;
}

GlobalTexture::GlobalTexture( GlobalTexture &&other )
{
    m_path = std::move( other.m_path );
    m_data = other.m_data ? std::move( other.m_data ) : 0;
}

GlobalTexture::~GlobalTexture()
{
}

GlobalTexture &GlobalTexture::operator=( const GlobalTexture &other )
{
    if ( &other == this )
        return *this;

    m_path = other.m_path;
    m_data = other.m_data ? make_unique<RawTextureData>( *other.m_data.get() ) : 0;

    return *this;
}

GlobalTexture &GlobalTexture::operator=( GlobalTexture &&other )
{
    if ( &other == this )
        return *this;
    
    m_path = std::move( other.m_path );
    m_data = other.m_data ? std::move( other.m_data ) : 0;

    return *this;
}
