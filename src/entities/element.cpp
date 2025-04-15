#include "element.h"
#include "render/mesh.h"
#include "render/window.h"


Element::Element( const std::shared_ptr<Window> &container, std::unique_ptr<Transform> &&transform, vector<std::unique_ptr<Renderable>> &&Elements ) :
    Renderable( container, std::move( transform ), std::move( Elements ) )
{
}

Element::Element( Element &&other ) :
    Renderable( std::move( other ) )
{
}

Element &Element::operator=( Element &&other )
{
    if ( &other == this )
        return other;

    Renderable::operator=( std::move( other ) );

    return *this;
}

//do not call base-class deconstructors: https://stackoverflow.com/questions/677620/do-i-need-to-explicitly-call-the-base-virtual-destructor
Element::~Element()
{
}

void Element::render() const
{
    if ( m_container )
        m_container->get_shader().SetShaderValue( "UI", false );
    else
        printf( "Element has persisted beyond the lifetime of the window it belongs to" );
    Renderable::render();
}


void UIElement::render() const
{
    if ( m_container )
        m_container->get_shader().SetShaderValue( "UI", true );
    else
        printf( "UI element has persisted beyond the lifetime of the window it belongs to" );
    Renderable::render();
}