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