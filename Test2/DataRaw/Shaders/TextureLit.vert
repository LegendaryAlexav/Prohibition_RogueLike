$input a_position, a_texcoord0, a_normal
$output v_texcoord0, v_normal, v_worldSpacePosition, v_lightClipSpacePosition

#include <bgfx_shader.sh>

uniform mat4 u_worldMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projMatrix;
uniform mat4 u_lightMatrix;
uniform vec4 u_uvScale;
uniform vec4 u_uvOffset;

void main()
{
    vec4 objectSpacePosition = vec4( a_position, 1 );
    vec4 worldSpacePosition = mul( u_worldMatrix, objectSpacePosition );
    vec4 viewSpacePosition = mul( u_viewMatrix, worldSpacePosition );
    vec4 clipSpacePosition = mul( u_projMatrix, viewSpacePosition );
    gl_Position = clipSpacePosition;

    v_lightClipSpacePosition = mul( u_lightMatrix, worldSpacePosition );

    v_texcoord0 = a_texcoord0 * u_uvScale.xy + u_uvOffset.xy;
    v_normal = mul( u_worldMatrix, vec4(a_normal,0) ).xyz;
    v_worldSpacePosition = worldSpacePosition.xyz;
}
