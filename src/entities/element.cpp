// This file is an adapted version of a file that originated in 2022 by the same author
// as part of a project that received periodic updates through 2024
// the changes in this version are
//      - un-deleting move constructor and defining move assignment operator
//      - refactoring pointers to smart pointers and reworking the associated logic
//      - const handling
//      - removing references to physics
// to see the original, go to https://github.com/solalunara/phys/blob/main/src/entities/element.cpp

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