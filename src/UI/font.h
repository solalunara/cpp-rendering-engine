#ifndef FONT_H
#define FONT_H
#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/glm.hpp>
#include <map>

using glm::ivec2;
using std::map;

#include "render/GlobalTexture.h"

struct Glyph 
{
    GlobalTexture   TextureID;
    ivec2           Size;
    ivec2           Bearing; // Offset from baseline to left/top of glyph
    unsigned int    Advance; // Offset to advance to next glyph
};

struct Font
{
    Font();
    Font( const char *FontPath );
    ~Font();
    map<char, Glyph> char_to_glyph;
private:
    FT_Library  library;
    FT_Face     face;
    FT_Error    error;
};
static const char *DefaultFont = "/usr/share/fonts/TTF/Hack-Regular.ttf";



#endif