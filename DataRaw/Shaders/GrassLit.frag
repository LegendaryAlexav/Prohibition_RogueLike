$input v_texcoord0, v_normal, v_worldSpacePosition

#include <bgfx_shader.sh>
#include "LightFunctions.sh"

uniform vec4 u_diffuseColor;

SAMPLER2D( u_textureColor, 0 );

void main()
{
    vec3 normal = normalize(v_normal);

    vec4 textureColor = texture2D( u_textureColor, v_texcoord0 );
    vec3 matColor = textureColor.rgb * u_diffuseColor.rgb;
    float alpha = textureColor.a;

    vec3 finalColor = matColor;
    for( int i=0; i<c_maxLights; i++ )
    {
        finalColor += getFinalLightColor( i, matColor, normal, v_worldSpacePosition );
    }


    gl_FragColor = vec4( finalColor, alpha );

    // Debug normals.
    //gl_FragColor = vec4( v_normal, 1 ) * vec4(1,0,1,1);

    // Discard transparent
    if( gl_FragColor.a < 0.5 )
        discard;
}
