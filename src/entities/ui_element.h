// This file is a new file based on a file that originated in 2022 by the same author
// as part of a project that received periodic updates through 2024
// this version primarily updates the constructors to either not use pointers at all or to use smart pointers
// to see the original, go to https://github.com/solalunara/phys/blob/main/src/entities/element.h

#ifndef UI_ELEMENT_H
#define UI_ELEMENT_H

#pragma once

#include "element.h"

struct UIElement :
    public Element
{
    UIElement( const std::shared_ptr<Window> &container, std::unique_ptr<Transform> &&transform, vector<unique_ptr<Renderable>> &&children );

    virtual void render() const;
};


#endif //UI_ELEMENT_H