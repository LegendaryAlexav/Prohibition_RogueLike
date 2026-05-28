$input a_position, a_texcoord0
$output v_texcoord0, v_worldSpacePosition

#include <bgfx_shader.sh>

uniform mat4 u_worldMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projMatrix;
uniform vec4 u_uvScale;
uniform vec4 u_uvOffset;
uniform vec4 u_time;

void main()
{
    vec4 objectSpacePosition = vec4( a_position, 1 );
    vec4 worldSpacePosition = mul( u_worldMatrix, objectSpacePosition );

    worldSpacePosition.y += sin( worldSpacePosition.x + u_time.x ) * 0.03;
    //worldSpacePosition.y += sin( worldSpacePosition.z + u_time.x );

    vec4 viewSpacePosition = mul( u_viewMatrix, worldSpacePosition );
    vec4 clipSpacePosition = mul( u_projMatrix, viewSpacePosition );
    gl_Position = clipSpacePosition;

    v_texcoord0 = a_texcoord0 * u_uvScale.xy + u_uvOffset.xy;
    v_worldSpacePosition = worldSpacePosition.xyz;
}
