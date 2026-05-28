#include "Framework.h"
#include "Light.h"

Light::Light(const vec3& position, const vec3& rotation, const vec3& color, float ambient)
    : m_position( position )
    , m_rotation( rotation )
    , m_color( color )
    , m_range( 0 )
    , m_ambient( ambient )
{
}

Light::Light(const vec3& position, const vec3& color, float range, float ambient)
    : m_position( position )
    , m_rotation( 0,0,0 )
    , m_color( color )
    , m_range( range )
    , m_ambient( ambient )
{
}

void Light::showDebugUI()
{
    if( ImGui::Begin("Lights") )
    {
        ImGui::PushID( this );
        if (ImGui::CollapsingHeader("Light"))
        {
            ImGui::DragFloat3( "Position", &m_position.x, 0.1f );
            ImGui::DragFloat3( "Rotation", &m_rotation.x, 0.1f );
            ImGui::ColorEdit3( "Color", &m_color.x );
            ImGui::DragFloat( "Range", &m_range, 0.1f, 0.0f, 100.0f );
            ImGui::DragFloat( "Ambient", &m_ambient, 0.01f, 0.0f, 1.0f );
        }
        ImGui::PopID();
    }
    ImGui::End();
}
