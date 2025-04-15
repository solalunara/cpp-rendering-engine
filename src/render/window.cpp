#include "window.h"
#include "mesh.h"
#include "texture.h"
#include "GlobalTexture.h"
#include "renderable.h"

#include <string>
#include <string.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define NEARCLIP 0.0001f
#define FARCLIP 100000.f

using glm::zero;
using glm::identity;
using glm::one;
using glm::vec3;
using glm::quat;

bool Window::s_glad_initialized = false;

Window::Window() :
    Window( WindowState::Windowed, 90, 100, 100, "window", vector<GlobalTexture>() )
{
}

Window::Window( WindowState state, float FOV, int xres, int yres, const char *name, vector<GlobalTexture> textures ) : 
    m_camera_transform( zero<vec3>(), identity<quat>(), one<vec3>() ),
    m_perspective( glm::perspectiveFov( (float)glm::radians(FOV), (float)xres, (float)yres, NEARCLIP, FARCLIP ) ),
    m_FOV( FOV ),
    m_name( name )
{
    glfwDefaultWindowHints();
    const GLFWvidmode* mode = glfwGetVideoMode( glfwGetPrimaryMonitor() );
    switch ( state )
    {
        case WindowState::Windowed:
        m_ID = glfwCreateWindow( xres, yres, name, NULL, NULL );
        break;

        case WindowState::Borderless:
        glfwWindowHint( GLFW_RED_BITS, mode->redBits );
        glfwWindowHint( GLFW_GREEN_BITS, mode->greenBits );
        glfwWindowHint( GLFW_BLUE_BITS, mode->blueBits );
        glfwWindowHint( GLFW_REFRESH_RATE, mode->refreshRate );
        glfwWindowHint( GLFW_AUTO_ICONIFY, GLFW_FALSE );
        m_ID = glfwCreateWindow( xres, yres, name, glfwGetPrimaryMonitor(), NULL );
        break;

        case WindowState::Fullscreen:
        glfwWindowHint( GLFW_RED_BITS, mode->redBits );
        glfwWindowHint( GLFW_GREEN_BITS, mode->greenBits );
        glfwWindowHint( GLFW_BLUE_BITS, mode->blueBits );
        glfwWindowHint( GLFW_REFRESH_RATE, mode->refreshRate );
        m_ID = glfwCreateWindow( xres, yres, name, glfwGetPrimaryMonitor(), NULL );
        break;

        default:
        printf( "Invalid window state - %i\n", (int)state );
        m_ID = 0;
        break;
    }

    if ( !ID )
    {
        const char *msg[ 1 ] { new char[ 1024 ] { 0 } };
        glfwGetError( msg );
        printf( "Couldn't create window - %s\n", msg[ 0 ] );
        printf( "Possible restart needed after driver update\n" );
    }

    glfwMakeContextCurrent( ID );
    glfwSetFramebufferSizeCallback( ID, ResizeCallback );
    glfwSetKeyCallback( ID, KeyCallback );
    if ( !s_glad_initialized )
    {
        s_glad_initialized = true;
        //requires a context to be active - has to be done after first window creation
        if ( !gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress ) )
            printf( "Couldn't initialize glad\n" );
    }

    m_shader.InitShader( ID );
    glfwSwapInterval( 1 );

    //this is needed to access this window object from the opengl callbacks (e.g. resize)
    glfwSetWindowUserPointer( ID, this );

    glEnable( GL_DEPTH_TEST );
	glEnable( GL_FRAMEBUFFER_SRGB );
	//glEnable( GL_CULL_FACE );
	glDepthFunc( GL_LESS );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );  

    //create local textures for every global texture instantiated
    for ( int i = 0; i < textures.size(); ++i )
        m_textures.push_back( std::make_unique<Texture>( textures[ i ].get_path().c_str(), *this ) );

    ResizeCallback( ID, xres, yres );
}

Window::~Window()
{
    glfwMakeContextCurrent( ID );
    m_elements.clear();
    m_textures.clear();
    glfwDestroyWindow( ID );

    //refresh the viewport on other windows
    /*
    for ( auto id_window_pair : s_id_to_window_map )
    {
        const WindowID other_window_id = id_window_pair.first;
        glfwMakeContextCurrent( other_window_id );
        int xres, yres;
        glfwGetFramebufferSize( other_window_id, &xres, &yres );
        ResizeCallback( other_window_id, xres, yres );
        id_window_pair.second.lock()->Render();
    }
    */
}

Window::Window( Window &&other ) :
    m_camera_transform( std::move( other.m_camera_transform ) )
{
    m_ID = other.m_ID;
    m_current_state = other.m_current_state;
    for ( int i = 0; i < sizeof( KeyFlags ) / sizeof( unsigned long long ); ++i )
        KeyFlags[ i ] = other.KeyFlags[ i ];
    m_name = other.m_name;
    m_shader = std::move( other.m_shader );

    m_elements = std::move( other.m_elements );
    m_textures = std::move( other.m_textures );

    m_FOV = other.m_FOV;
    m_perspective = other.m_perspective;
}

Window &Window::operator=( Window &&other )
{
    if ( &other == this )
        return *this;

    m_ID = other.m_ID;
    m_current_state = other.m_current_state;
    for ( int i = 0; i < sizeof( KeyFlags ) / sizeof( unsigned long long ); ++i )
        KeyFlags[ i ] = other.KeyFlags[ i ];
    m_name = other.m_name;
    m_shader = std::move( other.m_shader );

    m_elements = std::move( other.m_elements );
    m_textures = std::move( other.m_textures );

    m_FOV = other.m_FOV;
    m_perspective = other.m_perspective;
    m_camera_transform = std::move( other.m_camera_transform );

    return *this;
}

void Window::set_state( WindowState state, int xres, int yres )
{
    if ( state == m_current_state )
        return;

    //window hints don't actually do anything unless creating new window
    const GLFWvidmode* mode = glfwGetVideoMode( glfwGetPrimaryMonitor() );
    switch ( state )
    {
        case WindowState::Windowed:
        glfwSetWindowMonitor( ID, NULL, 0, 0, xres, yres, GLFW_DONT_CARE );
        break;

        case WindowState::Borderless:
        //glfwWindowHint( GLFW_RED_BITS, mode->redBits );
        //glfwWindowHint( GLFW_GREEN_BITS, mode->greenBits );
        //glfwWindowHint( GLFW_BLUE_BITS, mode->blueBits );
        glfwSetWindowAttrib( ID, GLFW_AUTO_ICONIFY, GLFW_FALSE );
        glfwSetWindowMonitor( ID, glfwGetPrimaryMonitor(), 0, 0, xres, yres, mode->refreshRate );
        break;

        case WindowState::Fullscreen:
        //glfwWindowHint( GLFW_RED_BITS, mode->redBits );
        //glfwWindowHint( GLFW_GREEN_BITS, mode->greenBits );
        //glfwWindowHint( GLFW_BLUE_BITS, mode->blueBits );
        glfwSetWindowAttrib( ID, GLFW_AUTO_ICONIFY, GLFW_TRUE );
        glfwSetWindowMonitor( ID, glfwGetPrimaryMonitor(), 0, 0, xres, yres, mode->refreshRate );
        break;

    }
}
void Window::render()
{
    m_shader.SetShaderValue( "CameraTransform", m_camera_transform.get_inverse_matrix() );
    m_shader.SetShaderValue( "Perspective", m_perspective );
    for ( int i = 0; i < m_elements.size(); ++i )
        m_elements[ i ]->render();

    //int x, y;
    //glfwGetFramebufferSize( ID, &x, &y );
    //ResizeCallback( ID, x, y );
}

void Window::set_key_flag( int key, bool set )
{
    if ( set )
        KeyFlags[ key / 64 ] |= 1 << ( key % 64 );
    else
        KeyFlags[ key / 64 ] &= ~( 1 << ( key % 64 ) );
}
bool Window::get_key_flag( int key )
{
    return KeyFlags[ key / 64 ] & ( 1 << ( key % 64 ) );
}

unsigned int Window::get_texture_opengl_id( GlobalTexture tex ) const
{
    for ( int i = 0; i < m_textures.size(); ++i )
        if ( m_textures[ i ]->get_path() == tex.get_path() )
            return m_textures[ i ]->id;
    return -1;
}

int Window::add_element(unique_ptr<Renderable> &&element)
{ 
    m_elements.push_back( std::move( element ) ); 
    return m_elements.size() - 1;
}

void Window::remove_element(int index)
{ 
    m_elements.erase( m_elements.begin() + index );
}

void ResizeCallback( WindowID window, int width, int height )
{
    glfwMakeContextCurrent( window );
    glViewport( 0, 0, width, height );
    Window *w = static_cast<Window *>( glfwGetWindowUserPointer( window ) );
    w->m_perspective = glm::perspectiveFov( (float)glm::radians( w->m_FOV ), (float)width, (float)height, NEARCLIP, FARCLIP );
}

void KeyCallback( WindowID window, int key, int scancode, int action, int mods )
{
    Window *w = static_cast<Window *>( glfwGetWindowUserPointer( window ) );
    switch ( action )
    {
        case GLFW_PRESS:
        w->set_key_flag( key, true );
        break;

        case GLFW_RELEASE:
        w->set_key_flag( key, false );
        break;
    }
}