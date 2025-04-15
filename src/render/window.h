#ifndef WINDOW_H
#define WINDOW_H

#pragma once

#include <vector>
#include <map>
#include <cstring>
#include <string>
#include <memory>
using std::vector;
using std::map;
using std::string;

#include "shader.h"
#include "transform.h"
#include "GlobalTexture.h"

struct GLFWmonitor;
struct GLFWwindow;
struct Mesh;
struct Texture;
struct Element;
struct PhysicsBaseObject;
struct Renderable;

enum class WindowState : unsigned char
{
    Windowed = 0,
    Borderless = 1,
    Fullscreen = 2,
};

typedef GLFWwindow *WindowID;

class Window
{
public:
    Window();
    Window( WindowState state, float FOV, int xres, int yres, const char *name, vector<GlobalTexture> textures );
    ~Window();

    Window( const Window & ) = delete;
    Window &operator =( const Window & ) = delete;

    Window( Window &&other );
    Window &operator =( Window &&other );

    void set_state( WindowState state, int xres, int yres );
    void render();

    void set_key_flag( int key, bool set );
    bool get_key_flag( int key );

    const WindowID &ID = m_ID;

    void set_this_shader_active() { m_shader.Use(); }
    unsigned int get_texture_opengl_id( GlobalTexture tex ) const;

    float get_fov() const { return m_FOV; }
    mat4 get_perspective() const { return m_perspective; }
    const Shader &get_shader() const { return m_shader; }
    Shader &get_shader() { return m_shader; }

    void set_perspective( const mat4 perspective ) { m_perspective = perspective; }
    void set_camera_position( const vec3 pos ) { m_camera_transform.set_abs_origin( pos ); }
    void move_camera_position( const vec3 delta ) { m_camera_transform.set_abs_origin( m_camera_transform.get_abs_origin() + delta ); }
    int add_element( std::shared_ptr<Renderable> element ) { m_elements.push_back( element ); return m_elements.size() - 1; }
    void remove_element( int index ) { m_elements.erase( m_elements.begin() + index ); }

private:
    WindowID m_ID;
    WindowState m_current_state;
    unsigned long long KeyFlags[ 6 ] { 0 };
    string m_name;
    Shader m_shader;
    float m_FOV;
    mat4 m_perspective;
    Transform m_camera_transform;
    vector<std::shared_ptr<Renderable>> m_elements;
    vector<std::unique_ptr<Texture>> m_textures;

    static bool s_glad_initialized;

    friend void ResizeCallback( WindowID window, int width, int height );
    friend void KeyCallback( WindowID window, int key, int scancode, int action, int mods );
};

void ResizeCallback( WindowID window, int width, int height );
void KeyCallback( WindowID window, int key, int scancode, int action, int mods );

#endif