#version 460 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture_diffuse1;

void main()
{
    FragColor = texture(texture_diffuse1, TexCoord);
    // FragColor = vec4(1,0,0, 1.0); // red
    // FragColor = vec4(0.627,0.627,0.627, 1.0); // Grey
}