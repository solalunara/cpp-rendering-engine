// This file is an adapted version of a file that originated in 2023 by the same author
// as part of a project that received periodic updates through 2024
// the file in that project was a derivative of a file originally written by Joey de Vries made avaliable via learnopengl.com
// where much of the original logic for interfacing with freetype was written
// this version primarily updates the constructors to either not use pointers at all or to use smart pointers
// to see the original, go to https://github.com/solalunara/phys/blob/main/src/entities/text.cpp
// to see the file by Joey de Vries, go to https://github.com/JoeyDeVries/LearnOpenGL/blob/master/src/7.in_practice/2.text_rendering/text_rendering.cpp

#include "text.h"
#include "render/mesh.h"
#include "render/window.h"
#include "UI/font.h"
#include "ui_element.h"
#include <cstring>

using std::make_unique;
using std::unique_ptr;

UIText::UIText( const char *text, Font font, float x, float y, float scale, vec3 color, const shared_ptr<Window> &container ) :
    UIElement( container, make_unique<Transform>( vec3( x, y, -1 ), glm::identity<quat>(), glm::one<vec3>() ), vector<unique_ptr<Renderable>>() )
{
    unsigned long long len = strlen( text );
    struct Data
    {
        Glyph ch;
        float xpos;
        float ypos;
        float w;
        float h;
    };
    Data data[ len ] {};

    float TextWidth = 0;

    float xnet = 0.f;
    for ( unsigned long long i = 0; i < len; ++i )
    {
        Glyph ch = font.char_to_glyph[ text[ i ] ];

        float xpos = xnet + ch.Bearing.x * scale;
        float ypos = (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        data[ i ] = Data{ ch, xpos, ypos, w, h };

        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        if ( i + 1 < len )
            xnet += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }

    TextWidth = xnet;

    for ( unsigned long long i = 0; i < len; ++i )
    {
        m_elements.push_back( make_unique<CharacterMesh>( vec2( -data[ i ].w / 2, -data[ i ].h / 2 ), vec2( data[ i ].w / 2, data[ i ].h / 2 ),
            data[ i ].ch.TextureID, make_unique<Transform>( vec3( data[ i ].xpos - TextWidth / 2, data[ i ].ypos, 0 ), glm::identity<quat>(), glm::one<vec3>() ), 
            container, color, text[ i ], this ) );
    }
    claim_all_children();
}

Text3D::Text3D( const char *text, Font font, float x, float y, float z, float scale, vec3 color, const shared_ptr<Window> &container ) :
    Element( container, make_unique<Transform>( vec3( x, y, z ), glm::identity<quat>(), glm::one<vec3>() ), vector<unique_ptr<Renderable>>() )
{    
    unsigned long long len = strlen( text );
    struct Data
    {
        Glyph ch;
        float xpos;
        float ypos;
        float w;
        float h;
    };
    Data data[ len ] {};

    float TextWidth = 0;

    float xnet = 0.f;
    for ( int i = 0; i < strlen( text ); ++i )
    {
        Glyph ch = font.char_to_glyph[ text[ i ] ];

        float xpos = xnet + ch.Bearing.x * scale;
        float ypos = (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        data[ i ] = Data{ ch, xpos, ypos, w, h };

        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        if ( i + 1 < len )
            xnet += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }

    TextWidth = xnet;

    for ( unsigned long long i = 0; i < len; ++i )
    {
        m_elements.push_back( make_unique<CharacterMesh>( vec2( -data[ i ].w / 2, -data[ i ].h / 2 ), vec2( data[ i ].w / 2, data[ i ].h / 2 ),
            data[ i ].ch.TextureID, make_unique<Transform>( vec3( data[ i ].xpos - TextWidth / 2, data[ i ].ypos, 0 ), glm::identity<quat>(), glm::one<vec3>() ), 
            container, color, text[ i ], this ) );
    }


}