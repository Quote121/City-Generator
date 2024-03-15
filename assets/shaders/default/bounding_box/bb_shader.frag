#version 460 core

out vec4 FragColor;

// Default white
uniform vec3 colour = vec3(1, 1, 1);

void main()
{
    FragColor = vec4(colour, 1.0);
}
