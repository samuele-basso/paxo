#version 460 core

layout(location = 0) out vec4 o_color;

in vec4 color;

void main()
{
    o_color = color;
}
