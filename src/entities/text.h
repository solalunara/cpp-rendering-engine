// This file is an adapted version of a file that originated in 2023 by the same author
// as part of a project that received periodic updates through 2024
// this version primarily updates the constructors to either not use pointers at all or to use smart pointers
// to see the original, go to https://github.com/solalunara/phys/blob/main/src/entities/text.h

#ifndef TEXT_H
#define TEXT_H
#pragma once

#include "ui_element.h"

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
