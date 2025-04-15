#include "sprite.h"
#include "render/mesh.h"

using std::make_unique;

Sprite::Sprite( const shared_ptr<Window> &container, GlobalTexture texture, glm::vec2 anchor_min, glm::vec2 anchor_max, unique_ptr<Transform> &&transform ) :
    UIElement( container, std::move( transform ), vector<unique_ptr<Renderable>>() )
{
    glm::quat rot_angle = glm::angleAxis( M_PIf, vec3( 1, 0, 0 ) );
    m_elements.push_back( make_unique<Mesh>( anchor_min, anchor_max, texture, make_unique<Transform>( glm::zero<vec3>(), rot_angle, glm::one<vec3>() ), container ) );
    claim_all_children();
}
