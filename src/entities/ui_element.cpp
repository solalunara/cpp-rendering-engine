// This file is a new file based on a file that originated in 2022 by the same author
// as part of a project that received periodic updates through 2024
// this version primarily updates the constructors to either not use pointers at all or to use smart pointers
// to see the original, go to https://github.com/solalunara/phys/blob/main/src/entities/element.cpp

#include "ui_element.h"

UIElement::UIElement( const std::shared_ptr<Window> &container, std::unique_ptr<Transform> &&transform, vector<unique_ptr<Renderable>> &&children ) :
    Element( container, std::move( transform ), std::move( children ) )
{
}

void UIElement::render() const
{
    if ( m_container )
        m_container->get_shader().SetShaderValue( "UI", true );
    else
        printf( "UI element has persisted beyond the lifetime of the window it belongs to" );
    Renderable::render();
}