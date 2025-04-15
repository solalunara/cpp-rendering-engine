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
