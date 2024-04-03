#pragma once
/*
    Sprite is just a 2D texture, it has position and rotation
    Most of the time it'll be a billboard to the player

    Scale it can only scale in x and y since its a flat texture
*/
#include <glm/glm.hpp>
#include <stb_image/stb_image.h>

#include <string>

#include <vertexArray.hpp>
#include <vertexBuffer.hpp>
#include <indexBuffer.hpp>

// Forward declaration
class Shader;
class BoundingBox;
class ResourceManager;

class SpriteRenderer
{
private:
    VertexArray* VAO;
    VertexBuffer* VBO;
    IndexBuffer* EBO;
    unsigned int matrixBuffer;

    Shader* spriteShader = nullptr;
    BoundingBox* spriteBoundingBox;

    std::string texturePath;
    unsigned int spriteTextureID;

    // Binding VAO's etc.
    void SetupSprite(float vertices[], unsigned int indices[]);
    
public:

    // constructor for texture
    SpriteRenderer(Shader* spriteShader_in, const std::string& spriteTexturePath_in);
    ~SpriteRenderer();

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
    void DrawInstance(std::vector<float>* matrices);
};
