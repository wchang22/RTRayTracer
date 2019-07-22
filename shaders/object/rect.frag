#version 450 core

in vec2 texture_coords;

out vec4 frag_color;

uniform sampler2D texture_sampler;

void main(void)
{
    frag_color = texture(texture_sampler, texture_coords);
}
