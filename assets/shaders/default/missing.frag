#version 460 core

out vec4 FragColor;
in vec2 TexCoord;

// uniform sampler2D texture1;

void main()
{
    FragColor = vec4(0.827,0.827,0.827, 1.0);
    
    // texture funtion takes the texture and the coords
    // FragColor = texture(texture1, TexCoord);
}