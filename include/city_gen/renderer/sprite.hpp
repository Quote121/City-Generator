#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>

#include "shader.hpp"
#include "bounding_box.hpp"
#include "resourceManager.hpp"

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
    BoundingBox* boundingBoxPtr;

    std::string texturePath;
    unsigned int spriteTextureID;

    // Binding VAO's etc.
    void SetupSprite(float vertices[], unsigned int indices[]);
    
public:

    // constructor for texture
    Sprite(Shader* spriteShader_in, const std::string& spriteTexturePath_in, BoundingBox* boundingBox);
    
    ~Sprite()
    {
        // Sprite shader is allocated memory in sprite_object.hpp, needs to be freed
        delete(spriteShader);
        glDeleteBuffers(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    // For shader hotswapping
    void SetSpriteShader(Shader* spriteShader_in)
        { spriteShader = spriteShader_in; }

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
