#ifndef SPRITE_H
#define SPRITE_H

#pragma once

#include "ui_element.h"

// a sprite is a UI element with a texture and no children
struct Sprite :
    public UIElement
{
    Sprite( const shared_ptr<Window> &container, GlobalTexture texture, glm::vec2 anchor_min, glm::vec2 anchor_max, unique_ptr<Transform> &&transform );

};

#endif ///ifndef SPRITE_H