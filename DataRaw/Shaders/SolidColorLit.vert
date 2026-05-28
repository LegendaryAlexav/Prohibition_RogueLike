$input a_position, a_normal
$output v_worldSpacePosition, v_normal

#include <bgfx_shader.sh>

uniform mat4 u_worldMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projMatrix;

void main()
{
    vec4 objectSpacePosition = vec4( a_position, 1 );
    vec4 worldSpacePosition = mul( u_worldMatrix, objectSpacePosition );
    vec4 viewSpacePosition = mul( u_viewMatrix, worldSpacePosition );
    vec4 clipSpacePosition = mul( u_projMatrix, viewSpacePosition );
    gl_Position = clipSpacePosition;

    v_normal =  mul( u_worldMatrix, vec4(a_normal, 0)).xyz;
    v_worldSpacePosition = worldSpacePosition.xyz;
}