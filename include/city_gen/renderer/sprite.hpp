#pragma once
/*
    Sprite is just a 2D texture, it has position and rotation
    Most of the time it'll be a billboard to the player

    Scale it can only scale in x and y since its a flat texture
*/
#include <glm/glm.hpp>
#include <stb_image/stb_image.h>

#include <string>

// Forward declaration
class Shader;
class BoundingBox;
class ResourceManager;

class Sprite
{
private:
    unsigned int VAO, VBO, EBO;

    Shader* spriteShader = nullptr;
    BoundingBox* spriteBoundingBox;

    std::string texturePath;
    unsigned int spriteTextureID;

    // Binding VAO's etc.
    void SetupSprite(float vertices[], unsigned int indices[]);
    
public:

    // constructor for texture
    Sprite(Shader* spriteShader_in, const std::string& spriteTexturePath_in);
    ~Sprite();

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

    inline BoundingBox* GetBoundingBox()
        { return spriteBoundingBox; }

    // Draw call for sprite
    void Draw();
};
