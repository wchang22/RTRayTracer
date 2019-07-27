#version 450 core

in vec2 texture_coords;

out vec4 frag_color;

uniform sampler2D texture_image;

void main(void)
{
    frag_color = texture2D(texture_image, texture_coords);
}
