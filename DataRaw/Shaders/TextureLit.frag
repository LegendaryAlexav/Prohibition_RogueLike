$input v_texcoord0, v_normal, v_worldSpacePosition

#include <bgfx_shader.sh>
#include "LightFunctions.sh"

SAMPLER2D( u_textureColor, 0 );

void main()
{
    vec3 normal = normalize(v_normal);

    vec4 textureColor = texture2D( u_textureColor, v_texcoord0 );
    vec3 matColor = textureColor.rgb;
    float alpha = textureColor.a;

    vec3 finalColor = vec3(0,0,0);
    for( int i=0; i<c_maxLights; i++ )
    {
        finalColor += getFinalLightColor( i, matColor, normal, v_worldSpacePosition );
    }

    gl_FragColor = vec4( finalColor, alpha );

    // Debug normals.
    //gl_FragColor = vec4( v_normal, 1 ) * vec4(1,0,1,1);
}
