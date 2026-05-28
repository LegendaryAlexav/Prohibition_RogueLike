$input v_texcoord0, v_worldSpacePosition

#include <bgfx_shader.sh>

SAMPLER2D( u_textureColor, 0 );

void main()
{
    vec4 colorArray[4] =
    {
        vec4( 1,0,0,1 ),
        vec4( 0,1,0,1 ),
        vec4( 0,0,1,1 ),
        vec4( 1,1,1,1 ),
    };

    float heightLimits[3] = { -2,0,2 }; // -5 to 5

    //vec4 color = texture2D( u_textureColor, v_texcoord0 );

    float height = v_worldSpacePosition.y;

    int index = 0;
    if( height > heightLimits[0] )
        index = 1;
    if( height > heightLimits[1] )
        index = 2;
    if( height > heightLimits[2] )
        index = 3;

    gl_FragColor = colorArray[index];
}
