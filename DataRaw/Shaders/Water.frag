$input v_texcoord0, v_worldSpacePosition

#include <bgfx_shader.sh>

SAMPLER2D( u_textureColor, 0 );

void main()
{
    //vec4 color = texture2D( u_textureColor, v_texcoord0 );
    vec2 uv = v_worldSpacePosition.xz / 20; // Complete hack, don't do this.

    vec4 color1 = texture2D( u_textureColor, uv );
    vec4 color2 = texture2D( u_textureColor, uv + vec2(0.2,0.2) );

    color1 += vec4(0,0,1,1);
    color2 *= vec4(0,0,0.6,1);

    gl_FragColor = color1 - color2;
}
