#include <cstdlib>
#include <iostream>
#include <vector> 
#include <map>
#include <filesystem>

using std::vector;
using std::filesystem::recursive_directory_iterator;
using std::make_unique;

#include "render/window.h"
#include "entities/cube.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

enum class InbuiltTexture : char
{
    NONE = 0,
    black = 1 << 0,
    dirt = 1 << 1,
    grass = 1 << 2,
    metal = 1 << 3,
    universe = 1 << 4,
};

void CheckOpenGLVersion()
{
    int Major, Minor;
    glGetIntegerv( GL_MAJOR_VERSION, &Major );
    glGetIntegerv( GL_MINOR_VERSION, &Minor );
    printf( "OpenGL version: %i.%i\n", Major, Minor );
    if ( Major < 3 || ( Major == 3 && Minor < 3 ) )
        printf( "OpenGL version not supported. Errors likely. Please update to 3.3\n" );
}

int WindowPreFrame( const std::shared_ptr<Window> &window )
{
    if ( glfwWindowShouldClose( window->ID ) )
        return 1;
    glfwMakeContextCurrent( window->ID );
    glClearColor( .2f, .3f, .3f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    return 0;
}

void WindowRender( const std::shared_ptr<Window> &window )
{
    //example key input detection
    if ( window->get_key_flag( GLFW_KEY_P ) )
    {
        window->set_key_flag( GLFW_KEY_P, false );
    }

    window->render();
}

void WindowPostFrame( const std::shared_ptr<Window> &window )
{
    glfwSwapBuffers( window->ID );
    glfwPollEvents();
}

int main( int argc, const char *argv[] )
{
    if ( !glfwInit() )
        printf( "Couldn't initialize glfw\n" );

    vector<GlobalTexture> WindowTextureList = {
        GlobalTexture( "./assets/textures/base/black.png" ),
        GlobalTexture( "./assets/textures/base/dirt.png" ),
        GlobalTexture( "./assets/textures/base/grass.png" ),
        GlobalTexture( "./assets/textures/base/metal.png" ),
        GlobalTexture( "./assets/textures/base/universe.png" )
    };

    // first window creation neccesary to init glad (gl____() functions)
    //          gl____() functions can't be initiated until a glfw context exists,
    //          but are also neccesary for certain operations on a window once created...
    //          so glad init is kind of stuck in the window constructor for now
    std::shared_ptr<Window> main = std::make_shared<Window>( WindowState::Windowed, 90.f, 1000, 768, "cpp project", WindowTextureList );

    CheckOpenGLVersion();

    map<InbuiltTexture, GlobalTexture> Textures = {
        { InbuiltTexture::black, GlobalTexture( "./assets/textures/base/black.png" ) },
        { InbuiltTexture::dirt, GlobalTexture( "./assets/textures/base/dirt.png" ) },
        { InbuiltTexture::grass, GlobalTexture( "./assets/textures/base/grass.png" ) },
        { InbuiltTexture::metal, GlobalTexture( "./assets/textures/base/metal.png" ) },
        { InbuiltTexture::universe, GlobalTexture( "./assets/textures/base/universe.png" ) },
    };

    std::vector<std::shared_ptr<Window>> Windows;
    Windows.push_back( main );
    // for now just treat executable arguments as commands to make new windows
    for ( int i = 1; i < argc; ++i )
    {
        std::shared_ptr<Window> w = std::make_shared<Window>( WindowState::Windowed, 90.f, 200, 152, argv[ i ], WindowTextureList );
        unique_ptr<Cube> c = make_unique<Cube>( glm::vec3( -.5f, -.5f, -.5f ), glm::vec3( .5f, .5f, .5f ), make_unique<Transform>( glm::vec3( 0 ), glm::identity<glm::quat>(), glm::vec3( 1 ) ), Textures[ InbuiltTexture::universe ], w );
        w->add_element( std::move( c ) );
        Windows.push_back( w );
    }

    //UIText *hello = new UIText( "helloo", 0, 0, 1.f, glm::vec3( 1.0f, 1.0f, 1.0f ), main );

    double t = glfwGetTime();
    double dt;

    vector<GlobalTexture> GroundTextures = { 
        Textures[ InbuiltTexture::dirt ], 
        Textures[ InbuiltTexture::dirt ], 
        Textures[ InbuiltTexture::dirt ], 
        Textures[ InbuiltTexture::dirt ], 
        Textures[ InbuiltTexture::grass ], 
        Textures[ InbuiltTexture::dirt ] };

    main->add_element( make_unique<Cube>( glm::vec3( -.5f, -.5f, -.5f ), glm::vec3( .5f, .5f, .5f ), make_unique<Transform>( glm::vec3( 0 ), glm::identity<glm::quat>(), glm::vec3( 1 ) ), Textures[ InbuiltTexture::universe ], main ) );
    //set the ground
    /*
    #define INT_GROUND_SIZE 10
    for ( int i = -INT_GROUND_SIZE; i <= INT_GROUND_SIZE; ++i )
        for ( int j = -INT_GROUND_SIZE; j <= INT_GROUND_SIZE; ++j )
        {
            shared_ptr<Cube> c = make_shared<Cube>( glm::vec3( -.5f, -.5f, -.5f ), glm::vec3( .5f, .5f, .5f ), make_unique<Transform>( glm::vec3( i, -3.f, j ), glm::identity<quat>(), glm::one<vec3>() ), GroundTextures, main );
            main->add_element( c );
        }
    */
    
    main->set_camera_position( vec3( 0, 0, 3 ) );

    vec3 player_mins = vec3( -.2f, -1.2f, -.2f );
    vec3 player_maxs = vec3(  .2f,   .2f,  .2f );

    while ( main )
    {
        //per frame for all windows
        double t_new = glfwGetTime();
        dt = t_new - t;
        t = t_new;

        //per frame per window
        for ( int i = 0; i < Windows.size(); ++i )
        {
            if ( WindowPreFrame( Windows[ i ] ) )
            {
                Windows.erase( Windows.begin() + i-- );
                if ( i != -1 )
                    printf( "Subwindow %d closed with exit code 0\n", i+1 );
                else
                {
                    printf( "Process closed with exit code 0\n" );
                    main = 0;
                }
                continue;
            }

            WindowRender( Windows[ i ] );

            WindowPostFrame( Windows[ i ] );
        }
    }
    glfwTerminate();
    return 0;
}
