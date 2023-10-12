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
    unsigned int VAO, VBO, EBO;

    Shader* spriteShader = nullptr;

    std::string texturePath;
    unsigned int spriteTextureID;

    // Binding VAO's etc.
    void SetupSprite(float vertices[], unsigned int indices[]);
    
public:

    // constructor for texture
    Sprite(Shader* spriteShader_in, const std::string& spriteTexturePath_in);
    
    ~Sprite()
    {
        // Sprite shader is allocated memory in sprite_object.hpp, needs to be freed
        delete(spriteShader);
    }

    // For shader hotswapping
    void SetSpriteShader(Shader* spriteShader_in);
    
    // Inline methods are defined where they are defined as inline
    inline Shader* GetSpriteShader()
        { return spriteShader; }

    inline unsigned int GetTextureId()
        { return spriteTextureID; }

    inline std::string GetTexturePath()
        { return texturePath; }

    // Draw call for sprite
    void Draw();
};
