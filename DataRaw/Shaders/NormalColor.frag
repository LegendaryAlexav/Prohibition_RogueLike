$input v_normal

#include <bgfx_shader.sh>

uniform vec4 u_diffuseColor;

void main()
{
    vec4 color = mul(vec4(v_normal,1.0), u_diffuseColor);
    gl_FragColor = color;
}
