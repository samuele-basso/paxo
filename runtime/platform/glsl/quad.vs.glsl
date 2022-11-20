#version 460 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in float a_tex_unit;
layout(location = 2) in vec4 a_color;
layout(location = 3) in vec2 a_tex_coord;

struct vertex_output
{
    vec4 color;
    vec2 tex_coord;
};

layout(location = 0) out vertex_output o_output;
layout(location = 2) out flat float v_tex_unit;

uniform mat4 u_view_projection;

void main()
{
    o_output.color = a_color;
    o_output.tex_coord = a_tex_coord;
    v_tex_unit = a_tex_unit;

    //vec4 temp = u_view_projection * vec4(a_position, 1.0f);
    //temp.z = 1.0f;
    //gl_Position = temp;

    gl_Position = u_view_projection * vec4(a_position, 1.0f);
}