#version 460 core

layout(location = 0) out vec4 o_color;

uniform sampler2DMS u_buffer;
uniform vec2 u_screen_size;
uniform int u_samples;

in vec2 tex_coord;

void main()
{
    ivec2 coords = ivec2(tex_coord.x * u_screen_size.x, tex_coord.y * u_screen_size.y);

    vec4 out_color = vec4(0.0f);
    for (int i = 0; i < u_samples; i++)
    {
        out_color += texelFetch(u_buffer, coords, i);
    }
    o_color = out_color / u_samples;
}