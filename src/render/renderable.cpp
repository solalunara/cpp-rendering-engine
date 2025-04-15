#include "renderable.h"
#include "mesh.h"
#include "window.h"


Renderable::Renderable( const std::shared_ptr<Window> &container, std::unique_ptr<Transform> &&transform, vector<std::unique_ptr<Renderable>> &&Elements ) :
    m_container( container.get() ), m_elements( std::move( Elements ) ), m_transform( std::move( transform ) ), m_parent( NULL )
{
    m_transform->m_owning_object = this;
    claim_all_children();
}

Renderable::Renderable( Renderable &&other ) :
    m_elements( std::move( other.m_elements ) ), m_transform( std::move( other.m_transform ) ), m_container( other.m_container ), m_parent( other.m_parent )
{
    m_transform->m_owning_object = this;
    claim_all_children();
}

Renderable &Renderable::operator=( Renderable &&other )
{
    if ( &other == this )
        return other;

    m_elements = std::move( other.m_elements );
    m_transform = std::move( other.m_transform );
    m_container = other.m_container;
    other.m_container = NULL;
    other.m_transform->m_owning_object = NULL;
    m_transform->m_owning_object = this;

    m_parent = other.m_parent;
    claim_all_children();

    return *this;
}

void Renderable::claim_all_children()
{
    for ( int i = 0; i < m_elements.size(); ++i )
        m_elements[ i ]->m_parent = this;
}

Renderable::~Renderable()
{
    //window owns a shared ptr to renderables - if destructor is being called we no longer exist within the window
}

void Renderable::render() const
{
    for ( int i = 0; i < m_elements.size(); ++i )
        m_elements[ i ]->render();
}
vector<vec3> Renderable::get_normals() const
{
    vector<vec3> normals;
    normals.reserve( m_elements.size() );
    for ( int i = 0; i < m_elements.size(); ++i )
    {
        if ( m_elements[ i ]->is_mesh() )
            normals.push_back( m_elements[ i ]->get_normal() );
        else
        {
            vector<vec3> childnormals = m_elements[ i ]->get_normals();
            for ( int j = 0; j < childnormals.size(); ++j )
                normals.push_back( childnormals[ j ] );
        }
    }
    normals.shrink_to_fit();
    return normals;
}
vector<vec3> Renderable::get_vertices( bool local ) const
{
    vector<vec3> verts;
    verts.reserve( 4 * m_elements.size() );
    for ( int i = 0; i < m_elements.size(); ++i )
    {
        if ( m_elements[ i ]->is_mesh() )
        {
            vector<vec3> meshverts = m_elements[ i ]->get_vertices();
            for ( int j = 0; j < meshverts.size(); ++j )
                verts.push_back( meshverts[ j ] );
        }
        else
        {
            vector<vec3> childverts = m_elements[ i ]->get_vertices();
            for ( int j = 0; j < childverts.size(); ++j )
                verts.push_back( childverts[ j ] );
        }
    }
    if ( local )
        for ( int i = 0; i < verts.size(); ++i )
                verts[ i ] = m_transform->world_to_local_point( verts[ i ] );
    verts.shrink_to_fit();
    return verts;
}
vector<float> Renderable::get_plane_dists() const
{
    vector<float> dists;
    dists.reserve( m_elements.size() );
    for ( int i = 0; i < m_elements.size(); ++i )
    {
        if ( m_elements[ i ]->is_mesh() )
        {
            float meshdist = m_elements[ i ]->get_plane_dist();
            dists.push_back( meshdist );
        }
        else
        {
            vector<float> childdists = m_elements[ i ]->get_plane_dists();
            for ( int j = 0; j < childdists.size(); ++j )
                dists.push_back( childdists[ j ] );
        }
    }
    dists.shrink_to_fit();
    return dists;
}

const mat4 Renderable::get_transform_matrix() const
{
    return m_transform->get_matrix();
}

const mat4 Renderable::get_inverse_transform_matrix() const
{
    return m_transform->get_inverse_matrix();
}

void Renderable::add_child( unique_ptr<Renderable> &&child )
{
    child->m_parent = this;
    m_elements.push_back( std::move( child ) );
}
