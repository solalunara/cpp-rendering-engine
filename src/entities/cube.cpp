#include "cube.h"
#include "render/window.h"
#include "render/GlobalTexture.h"
#include <string.h>

using std::unique_ptr;

Cube::Cube( vec3 mins, vec3 maxs, std::unique_ptr<Transform> &&transform, GlobalTexture texture, const std::shared_ptr<Window> &container ) :
    Element( container, std::move( transform ), vector<unique_ptr<Renderable>>() ), mins( mins ), maxs( maxs ),
    front_tex( texture ), back_tex( texture ), left_tex( texture ), right_tex( texture ), top_tex( texture ), bottom_tex( texture )
{
    m_elements.push_back( std::make_unique<Mesh>( vec2( mins.x, mins.y ), vec2( maxs.x, maxs.y ), texture, std::move( std::make_unique<Transform>( vec3( 0, 0, maxs.z ), glm::identity<quat>(), glm::one<vec3>() ) ), container ) );
    m_elements.push_back( std::make_unique<Mesh>( vec2( mins.x, mins.y ), vec2( maxs.x, maxs.y ), texture, std::move( std::make_unique<Transform>( vec3( 0, 0, mins.z ), glm::angleAxis(  M_PIf  , vec3( 0, 1, 0 ) ), glm::one<vec3>() ) ), container ) );
    m_elements.push_back( std::make_unique<Mesh>( vec2( mins.z, mins.y ), vec2( maxs.z, maxs.y ), texture, std::move( std::make_unique<Transform>( vec3( mins.x, 0, 0 ), glm::angleAxis(  M_PI_2f, vec3( 0, 1, 0 ) ), glm::one<vec3>() ) ), container ) );
    m_elements.push_back( std::make_unique<Mesh>( vec2( mins.z, mins.y ), vec2( maxs.z, maxs.y ), texture, std::move( std::make_unique<Transform>( vec3( maxs.x, 0, 0 ), glm::angleAxis( -M_PI_2f, vec3( 0, 1, 0 ) ), glm::one<vec3>() ) ), container ) );
    m_elements.push_back( std::make_unique<Mesh>( vec2( mins.x, mins.z ), vec2( maxs.x, maxs.z ), texture, std::move( std::make_unique<Transform>( vec3( 0, maxs.y, 0 ), glm::angleAxis(  M_PI_2f, vec3( 1, 0, 0 ) ), glm::one<vec3>() ) ), container ) );
    m_elements.push_back( std::make_unique<Mesh>( vec2( mins.x, mins.z ), vec2( maxs.x, maxs.z ), texture, std::move( std::make_unique<Transform>( vec3( 0, mins.y, 0 ), glm::angleAxis( -M_PI_2f, vec3( 1, 0, 0 ) ), glm::one<vec3>() ) ), container ) );
    claim_all_children();
}
Cube::Cube( vec3 mins, vec3 maxs, std::unique_ptr<Transform> &&transform, vector<GlobalTexture> textures, const std::shared_ptr<Window> &container ) :
    Element( container, std::move( transform ), vector<unique_ptr<Renderable>>() ), mins( mins ), maxs( maxs ),
    front_tex( textures[ 0 ] ), back_tex( textures[ 1 ] ), left_tex( textures[ 2 ] ), right_tex( textures[ 3 ] ), top_tex( textures[ 4 ] ), bottom_tex( textures[ 5 ] )
{
    if ( textures.size() != 6 )
        printf( "Error - wrong number of global textures passed to cube" );
    m_elements.push_back( std::make_unique<Mesh>( vec2( mins.x, mins.y ), vec2( maxs.x, maxs.y ), textures[ 0 ], std::move( std::make_unique<Transform>( vec3( 0, 0, maxs.z ), glm::identity<quat>(), glm::one<vec3>() ) ), container ) );
    m_elements.push_back( std::make_unique<Mesh>( vec2( mins.x, mins.y ), vec2( maxs.x, maxs.y ), textures[ 1 ], std::move( std::make_unique<Transform>( vec3( 0, 0, mins.z ), glm::angleAxis(  M_PIf  , vec3( 0, 1, 0 ) ), glm::one<vec3>() ) ), container ) );
    m_elements.push_back( std::make_unique<Mesh>( vec2( mins.z, mins.y ), vec2( maxs.z, maxs.y ), textures[ 2 ], std::move( std::make_unique<Transform>( vec3( mins.x, 0, 0 ), glm::angleAxis(  M_PI_2f, vec3( 0, 1, 0 ) ), glm::one<vec3>() ) ), container ) );
    m_elements.push_back( std::make_unique<Mesh>( vec2( mins.z, mins.y ), vec2( maxs.z, maxs.y ), textures[ 3 ], std::move( std::make_unique<Transform>( vec3( maxs.x, 0, 0 ), glm::angleAxis( -M_PI_2f, vec3( 0, 1, 0 ) ), glm::one<vec3>() ) ), container ) );
    m_elements.push_back( std::make_unique<Mesh>( vec2( mins.x, mins.z ), vec2( maxs.x, maxs.z ), textures[ 4 ], std::move( std::make_unique<Transform>( vec3( 0, maxs.y, 0 ), glm::angleAxis(  M_PI_2f, vec3( 1, 0, 0 ) ), glm::one<vec3>() ) ), container ) );
    m_elements.push_back( std::make_unique<Mesh>( vec2( mins.x, mins.z ), vec2( maxs.x, maxs.z ), textures[ 5 ], std::move( std::make_unique<Transform>( vec3( 0, mins.y, 0 ), glm::angleAxis( -M_PI_2f, vec3( 1, 0, 0 ) ), glm::one<vec3>() ) ), container ) );
    claim_all_children();
}