#include "UI/panel.h"
#include "entities/text.h"
#include "font.h"
#include "entities/ui_element.h"

using std::make_unique;

UIPanelTitlebar::UIPanelTitlebar( const char *title, float x, float y, float scale, float depth, const shared_ptr<Window> &container ) :
    UIElement( container, make_unique<Transform>( vec3( x, y, -1 ), glm::identity<quat>(), vec3( scale ) ), vector<unique_ptr<Renderable>>() ), m_panel_title( panel_title )
{
    //TODO: scale will need to be adjusted
    m_elements.push_back( make_unique<UIText>( title, Font(), x, y, scale, vec3( 1 ), container ) );
}