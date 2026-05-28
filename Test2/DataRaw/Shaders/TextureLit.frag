$input v_texcoord0, v_normal, v_worldSpacePosition, v_lightClipSpacePosition

#include <bgfx_shader.sh>
#include "LightFunctions.sh"

SAMPLER2D( u_textureColor, 0 );
SAMPLER2D( u_textureLightDepth, 1 );

void main()
{
    vec3 normal = normalize(v_normal);

    vec4 textureColor = texture2D( u_textureColor, v_texcoord0 );
    vec3 matColor = textureColor.rgb;
    float alpha = 1;

    vec3 finalColor = vec3(0,0,0);

    // Light 0 is a shadow-casting directional light.
    {
        // Get the light depth from the texture.
        vec2 lightUV = v_lightClipSpacePosition.xy / 2.0 + 0.5;
        lightUV = clamp(lightUV, vec2(0,0), vec2(1,1) );
        vec4 lightTextureDepth = texture2D( u_textureLightDepth, lightUV );
        float lightDepth = v_lightClipSpacePosition.z / 2.0 + 0.5;

        if( lightTextureDepth.r + 0.00001 < lightDepth )
        {
            // Only add ambient if the light hit something before reaching our pixel.
            finalColor += u_lightColor[0].rgb * matColor * u_ambientPerc[0].x;
        }
        else
        {
            finalColor += getFinalLightColor_Directional( 0, matColor, normal, v_worldSpacePosition );
        }
    }

    for( int i=1; i<c_maxLights; i++ )
    {
        finalColor += getFinalLightColor_Point( i, matColor, normal, v_worldSpacePosition );
    }

    gl_FragColor = vec4( finalColor, alpha );

    // Debug normals.
    //gl_FragColor = vec4( v_normal, 1 ) * vec4(1,0,1,1);
}
