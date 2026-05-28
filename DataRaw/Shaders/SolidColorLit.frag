$input v_worldSpacePosition, v_normal

#include <bgfx_shader.sh>
#include "LightFunctions.sh"

uniform vec4 u_diffuseColor;

void main()
{
    vec3 normal = normalize(v_normal);

    vec3 matColor = u_diffuseColor.rgb;
    float alpha = 1;

    vec3 finalColor = vec3(0,0,0);
    for(int i = 0; i<c_maxLights; i++)
    {
        finalColor += getFinalLightColor(i,matColor, v_normal, v_worldSpacePosition);
    }

    gl_FragColor = vec4(finalColor, alpha);

    // Debug normals
    //gl_FragColor = vec4(normal, 1) * vec4(1,0,1,1);
    
}

