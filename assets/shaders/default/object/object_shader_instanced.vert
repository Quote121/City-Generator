#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord; 
// each layout has the max size of a vec4 so this mat 4 uses layout 3,4,5,6 for the whole matrix
layout (location = 3) in mat4 model;

uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

void main()
{
	FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;

    TexCoord = aTexCoord;    
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}
