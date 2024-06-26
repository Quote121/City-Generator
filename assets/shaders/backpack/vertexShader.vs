#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 localCenterPos;
uniform vec2 textureScale = vec2(1.0, 1.0);

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;

    // TexCoord = aTexCoord * vec2(10, 10);
    TexCoord = aTexCoord * textureScale;
    gl_Position = projection * view * model * vec4(aPos-localCenterPos, 1.0);
}
