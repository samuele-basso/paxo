#version 460 core

layout (location = 0) in vec3 a_world_position;
layout (location = 1) in vec4 a_color;

out vec4 color;

uniform mat4 u_view_projection;

void main()
{
    color = a_color;
    gl_Position = u_view_projection * vec4(a_world_position, 1.0);
}