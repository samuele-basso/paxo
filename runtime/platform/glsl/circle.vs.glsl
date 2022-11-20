#version 460 core

layout(location = 0) in vec3 a_world_position;
layout(location = 1) in float a_thickness;
layout(location = 2) in vec4 a_color;
layout(location = 3) in vec2 a_local_space;
layout(location = 4) in float a_fade;

struct vertex_output
{
    vec4 color;
    vec2 local_space;
    float thickness;
    float fade;
};

uniform mat4 u_view_projection;

layout (location = 0) out vertex_output o_output;

void main()
{
    o_output.color = a_color;
    o_output.local_space = a_local_space;
    o_output.thickness = a_thickness;
    o_output.fade = a_fade;
    gl_Position = u_view_projection * vec4(a_world_position, 1.0);
}