#pragma once

#include <vector>
#include <string>

#include "shader.hpp"

/*
    Sprite is just a 2D texture, it has position and rotation
    Most of the time it'll be a billboard to the player

    Scale it can only scale in x and y since its a flat texture
*/


class Sprite
{
private:
    unsigned int VAO, VBO;

    Shader* spriteShader = nullptr;

    std::string texturePath;
    unsigned int spriteTextureID;

    // Binding VAO's etc.
    void SetupSprite(float vertices[]);
    
public:

    // constructor for texture
    Sprite(Shader* spriteShader_in, const std::string& spriteTexturePath_in);

    // For shader hotswapping
    void SetSpriteShader(Shader* spriteShader_in);

    inline Shader* GetSpriteShader();

    // Draw call for sprite
    void Draw();
};
