#version 460 core

out vec4 FragColor;

uniform vec3 colour;

void main()
{
    // FragColor = vec4(colour, 1.0);
    FragColor = vec4(vec3(1, 1, 1), 1.0); // White

}
