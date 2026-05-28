$input a_position, a_texcoord0, a_normal
$output v_texcoord0, v_normal, v_worldSpacePosition

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
    // To make the Grass sway back and forth on how it is facing
    //objectSpacePosition.z += (sin( u_time.x ) * objectSpacePosition.y)/10;
    vec4 worldSpacePosition = mul( u_worldMatrix, objectSpacePosition );
    worldSpacePosition.x += (sin( u_time.x ) * objectSpacePosition.y)/10;


    vec4 viewSpacePosition = mul( u_viewMatrix, worldSpacePosition );
    vec4 clipSpacePosition = mul( u_projMatrix, viewSpacePosition );
    gl_Position = clipSpacePosition;

    v_texcoord0 = a_texcoord0 * u_uvScale.xy + u_uvOffset.xy;
    v_normal = mul( u_worldMatrix, vec4(a_normal,0) ).xyz;
    v_worldSpacePosition = worldSpacePosition.xyz;
}
