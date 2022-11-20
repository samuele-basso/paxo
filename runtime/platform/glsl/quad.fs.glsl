#version 460 core

layout(location = 0) out vec4 color;

struct vertex_output
{
    vec4 color;
    vec2 tex_coord;
};

layout(location = 0) in vertex_output i_input;
layout(location = 2) in flat float v_tex_unit;

uniform sampler2D u_textures[32];

void main()
{
    vec4 tex_color = i_input.color;

    switch (int(v_tex_unit))
    {
        case  0: tex_color *= texture(u_textures[0], i_input.tex_coord); break;
        case  1: tex_color *= texture(u_textures[1], i_input.tex_coord); break;
        case  2: tex_color *= texture(u_textures[2], i_input.tex_coord); break;
        case  3: tex_color *= texture(u_textures[3], i_input.tex_coord); break;
        case  4: tex_color *= texture(u_textures[4], i_input.tex_coord); break;
        case  5: tex_color *= texture(u_textures[5], i_input.tex_coord); break;
        case  6: tex_color *= texture(u_textures[6], i_input.tex_coord); break;
        case  7: tex_color *= texture(u_textures[7], i_input.tex_coord); break;
        case  8: tex_color *= texture(u_textures[8], i_input.tex_coord); break;
        case  9: tex_color *= texture(u_textures[9], i_input.tex_coord); break;
        case 10: tex_color *= texture(u_textures[10], i_input.tex_coord); break;
        case 11: tex_color *= texture(u_textures[11], i_input.tex_coord); break;
        case 12: tex_color *= texture(u_textures[12], i_input.tex_coord); break;
        case 13: tex_color *= texture(u_textures[13], i_input.tex_coord); break;
        case 14: tex_color *= texture(u_textures[14], i_input.tex_coord); break;
        case 15: tex_color *= texture(u_textures[15], i_input.tex_coord); break;
        case 16: tex_color *= texture(u_textures[16], i_input.tex_coord); break;
        case 17: tex_color *= texture(u_textures[17], i_input.tex_coord); break;
        case 18: tex_color *= texture(u_textures[18], i_input.tex_coord); break;
        case 19: tex_color *= texture(u_textures[19], i_input.tex_coord); break;
        case 20: tex_color *= texture(u_textures[20], i_input.tex_coord); break;
        case 21: tex_color *= texture(u_textures[21], i_input.tex_coord); break;
        case 22: tex_color *= texture(u_textures[22], i_input.tex_coord); break;
        case 23: tex_color *= texture(u_textures[23], i_input.tex_coord); break;
        case 24: tex_color *= texture(u_textures[24], i_input.tex_coord); break;
        case 25: tex_color *= texture(u_textures[25], i_input.tex_coord); break;
        case 26: tex_color *= texture(u_textures[26], i_input.tex_coord); break;
        case 27: tex_color *= texture(u_textures[27], i_input.tex_coord); break;
        case 28: tex_color *= texture(u_textures[28], i_input.tex_coord); break;
        case 29: tex_color *= texture(u_textures[29], i_input.tex_coord); break;
        case 30: tex_color *= texture(u_textures[30], i_input.tex_coord); break;
        case 31: tex_color *= texture(u_textures[31], i_input.tex_coord); break;
    }
    color = tex_color;
}