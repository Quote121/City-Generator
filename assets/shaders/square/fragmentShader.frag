#version 460 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture_diffuse1;
uniform vec4 colour = {0.0, 0.0, 0.0, 1.0};

void main()
{
    // FragColor = texture(texture_diffuse1, TexCoord);
    FragColor = colour;
}