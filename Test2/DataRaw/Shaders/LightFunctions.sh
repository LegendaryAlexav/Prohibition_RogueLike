uniform vec4 u_specularPower;

uniform vec4 u_cameraPosition;

#define c_maxLights 4

uniform vec4 u_lightPos[c_maxLights];
uniform vec4 u_lightColor[c_maxLights];
uniform vec4 u_lightRange[c_maxLights];
uniform vec4 u_ambientPerc[c_maxLights];

vec3 getFinalLightColor_Directional(int lightIndex, vec3 matColor, vec3 normal, vec3 worldSpacePosition)
{
    // Light basics.
    vec3 dirToLight = -u_lightPos[lightIndex].xyz;
    float distToLight = length(dirToLight);
    vec3 normalizedDirToLight = dirToLight / distToLight;

    // Diffuse
    float diffusePerc = max( 0, dot(normal, normalizedDirToLight) );

    // Specular
    vec3 dirToCamera = u_cameraPosition.xyz - worldSpacePosition;
    vec3 normalizedDirToCamera = normalize( dirToCamera );
    vec3 halfVector = normalizedDirToCamera + normalizedDirToLight;
    vec3 normalizedHalfVector = normalize( halfVector );

    float specularPerc = max( 0, dot(normal, normalizedHalfVector) );
    specularPerc = pow( specularPerc, u_specularPower.x );

    // Final light values
    vec3 ambientColor = u_lightColor[lightIndex].rgb * matColor * u_ambientPerc[lightIndex].x;
    vec3 diffuseColor = u_lightColor[lightIndex].rgb * matColor * diffusePerc;
    vec3 specularColor = u_lightColor[lightIndex].rgb * specularPerc;

    vec3 color = ambientColor + (diffuseColor + specularColor);

    return color;
}

vec3 getFinalLightColor_Point(int lightIndex, vec3 matColor, vec3 normal, vec3 worldSpacePosition)
{
    // Light basics.
    vec3 dirToLight = u_lightPos[lightIndex].xyz - worldSpacePosition;
    float distToLight = length(dirToLight);
    vec3 normalizedDirToLight = dirToLight / distToLight;

    // Falloff
    float falloffPerc = 1.0 - (distToLight / u_lightRange[lightIndex].x);
    falloffPerc = saturate( falloffPerc );
    falloffPerc = pow( falloffPerc, 0.5 );

    // Diffuse
    float diffusePerc = max( 0, dot(normal, normalizedDirToLight) );

    // Specular
    vec3 dirToCamera = u_cameraPosition.xyz - worldSpacePosition;
    vec3 normalizedDirToCamera = normalize( dirToCamera );
    vec3 halfVector = normalizedDirToCamera + normalizedDirToLight;
    vec3 normalizedHalfVector = normalize( halfVector );

    float specularPerc = max( 0, dot(normal, normalizedHalfVector) );
    specularPerc = pow( specularPerc, u_specularPower.x );

    // Final light values
    vec3 ambientColor = u_lightColor[lightIndex].rgb * matColor * u_ambientPerc[lightIndex].x;
    vec3 diffuseColor = u_lightColor[lightIndex].rgb * matColor * diffusePerc;
    vec3 specularColor = u_lightColor[lightIndex].rgb * specularPerc;

    vec3 color = ambientColor + (diffuseColor + specularColor) * falloffPerc;

    return color;
}
