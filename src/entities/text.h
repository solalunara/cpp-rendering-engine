#ifndef TEXT_H
#define TEXT_H
#pragma once

#include "element.h"

struct CharacterMesh;
struct Font;


struct UIText :
    public UIElement
{
    UIText( const char *text, Font font, float x, float y, float scale, vec3 color, const shared_ptr<Window> &container );
};

struct Text3D :
    public Element
{
    Text3D( const char *text, Font font, float x, float y, float z, float scale, vec3 color, const shared_ptr<Window> &container );
};

#endif
