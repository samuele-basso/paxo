#version 460 core

layout(location = 0) out vec4 o_color;

struct vertex_output
{
    vec4 color;
    vec2 local_space;
    float thickness;
    float fade;
};

layout (location = 0) in vertex_output i_input;

void main()
{
    float dist = sqrt(dot(i_input.local_space, i_input.local_space));
    float t = i_input.thickness;
    if (dist > 1.0 || dist < 1.0 - t - i_input.fade)
        discard;

    float alpha = 1.0 - smoothstep(1.0f - i_input.fade, 1.0f, dist);
    alpha *= smoothstep(1.0 - t - i_input.fade, 1.0 - t, dist);
    o_color = i_input.color;
    o_color.a = i_input.color.a * alpha;
}