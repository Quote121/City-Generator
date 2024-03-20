#include <sprite.hpp>

#include <shader.hpp>
#include <bounding_box.hpp>
#include <resourceManager.hpp>
#include <config.hpp>
#include <renderer.hpp>
#include <glad/glad.h>

Sprite::Sprite(Shader* spriteShader_in, const std::string& filename) : texturePath{filename}
{
    spriteBoundingBox = new BoundingBox();
    TextureInfo* textureInfo = ResourceManager::getInstance()->LoadTexture(filename, true);

    float x, y;

    unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    float fHeight, fWidth;
    fHeight = static_cast<float>(textureInfo->height);
    fWidth = static_cast<float>(textureInfo->width);

    // here we get the ratio of width and height of the texture
    // so the texture isnt strange
    if (textureInfo->width > textureInfo->height)
    {
        x = 1.0f;
        y = (fHeight/fWidth);
    }
    else if (fHeight > fWidth)
    {
        x = (fWidth/fHeight);
        y = 1.0f;
    }
    else // width == height
    {
        x = 1.0f; y = 1.0f;
    }

    float vertices[] = {
    // positions           // texture coords
         x,  y, 0.0f,   1.0f, 1.0f,   // top right
         x, -y, 0.0f,   1.0f, 0.0f,   // bottom right
        -x, -y, 0.0f,   0.0f, 0.0f,   // bottom left
        -x,  y, 0.0f,   0.0f, 1.0f    // top left 
    };

    // Stream the verts to the bounding box
    spriteBoundingBox->StreamVertexUpdate(x, y, 0);
    spriteBoundingBox->StreamVertexUpdate(x, -y, 0);
    spriteBoundingBox->StreamVertexUpdate(-x, -y, 0);
    spriteBoundingBox->StreamVertexUpdate(-x, y, 0);

    // textureID to be stored
    spriteTextureID = textureInfo->textureID;
    // shader to be stored
    spriteShader = spriteShader_in;

    SetupSprite(vertices, indices);
}


Sprite::~Sprite()
{
    // Sprite shader is allocated memory in sprite_object.hpp, needs to be freed
    delete(spriteShader);
    delete(spriteBoundingBox);

    delete(VAO);
    delete(VBO);
    delete(EBO);
}


// Setup vertex and buffer arrays
void Sprite::SetupSprite(float vertices[], unsigned int indices[])
{
    VAO = new VertexArray();
    VBO = new VertexBuffer();
    EBO = new IndexBuffer();

    VertexBufferLayout vbl;
    vbl.AddFloat(3); // xyz
    vbl.AddFloat(2); // tex coords
    
    VBO->SetData<float>(vertices, 20);
    VAO->AddBuffer(VBO, &vbl);

    EBO->SetData(indices, 6);
}

// Make the OpenGL draw call
void Sprite::Draw()
{
    glActiveTexture(GL_TEXTURE0); // Associate the texture to GL_TEXTURE0 texture unit
    glBindTexture(GL_TEXTURE_2D, spriteTextureID);
    spriteShader->setInt("texture1", 0);

    Renderer::GetInstance()->DrawIndices(VAO, EBO);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind as models without textures will bind it 
}

