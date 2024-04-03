#include <sprite.hpp>

#include <shader.hpp>
#include <bounding_box.hpp>
#include <resourceManager.hpp>
#include <config.hpp>
#include <renderer.hpp>
#include <glad/glad.h>

SpriteRenderer::SpriteRenderer(Shader* spriteShader_in, const std::string& filename) : texturePath{filename}
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
    // positions        // Normals                        // texture coords
         x,  y, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,   // top right
         x, -y, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,   // bottom right
        -x, -y, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -x,  y, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f    // top left 
    };

    // Stream the verts to the bounding box
    spriteBoundingBox->StreamVertexUpdate(x, y, 0);
    spriteBoundingBox->StreamVertexUpdate(x, -y, 0);
    spriteBoundingBox->StreamVertexUpdate(-x, -y, 0);
    spriteBoundingBox->StreamVertexUpdate(-x, y, 0);
    spriteBoundingBox->SetupBuffers(); // Update the opengl buffers

    // textureID to be stored
    spriteTextureID = textureInfo->textureID;
    // shader to be stored
    spriteShader = spriteShader_in;

    SetupSprite(vertices, indices);
}


SpriteRenderer::~SpriteRenderer()
{
    glDeleteBuffers(1, &matrixBuffer);

    delete(spriteBoundingBox);

    delete(VAO);
    delete(VBO);
    delete(EBO);
}


// Setup vertex and buffer arrays
void SpriteRenderer::SetupSprite(float vertices[], unsigned int indices[])
{
    VAO = new VertexArray();
    VBO = new VertexBuffer();
    EBO = new IndexBuffer();
    glGenBuffers(1, &matrixBuffer);

    VertexBufferLayout vbl;
    vbl.AddFloat(3); // xyz
    vbl.AddFloat(3); // normals
    vbl.AddFloat(2); // tex coords
    
    VBO->SetData<float>(vertices, 32);
    VAO->AddBuffer(VBO, &vbl);

    EBO->SetData(indices, 6);
}

// Make the OpenGL draw call
void SpriteRenderer::Draw()
{
    glActiveTexture(GL_TEXTURE0); // Associate the texture to GL_TEXTURE0 texture unit
    glBindTexture(GL_TEXTURE_2D, spriteTextureID);
    spriteShader->setInt("texture1", 0);

    Renderer::GetInstance()->DrawIndices(VAO, EBO);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind as models without textures will bind it 
}

void SpriteRenderer::DrawInstance(std::vector<float>* matrices)
{
    // Bind texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, spriteTextureID);
    this->spriteShader->setInt("texture1", 0);

    VAO->Bind();
    // Bind matrices
    glBindBuffer(GL_ARRAY_BUFFER, this->matrixBuffer);
    glBufferData(GL_ARRAY_BUFFER, matrices->size() * sizeof(float), matrices->data(), GL_STATIC_DRAW);
    

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(4*sizeof(float)));

    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(8*sizeof(float)));

    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(12*sizeof(float)));

    // Configure them as instanced matrices
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    // glBindVertexArray(VAO);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    EBO->Bind();

    // Always drawing 4 indices
    glDrawElementsInstanced(GL_TRIANGLES, EBO->GetCount(), GL_UNSIGNED_INT, nullptr, matrices->size()/16);

    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

