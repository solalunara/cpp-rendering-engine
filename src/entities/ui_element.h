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