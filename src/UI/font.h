// This file is an adapted version of a file that originated in 2023 by the same author
// as part of a project that received periodic updates through 2024
// the file in that project was a derivative of a file originally written by Joey de Vries made avaliable via learnopengl.com
// where much of the original logic for interfacing with freetype was written
// the only change in this version is removing the pointer to globaltexture in glyph
// to see the original, go to https://github.com/solalunara/phys/blob/main/src/UI/font.h
// to see the file by Joey de Vries, go to https://github.com/JoeyDeVries/LearnOpenGL/blob/master/src/7.in_practice/2.text_rendering/text_rendering.cpp

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