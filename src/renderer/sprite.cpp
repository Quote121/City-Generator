#include <sprite.hpp>

#include <shader.hpp>
#include <bounding_box.hpp>
#include <resourceManager.hpp>
#include <config.hpp>

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
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}


// Setup vertex and buffer arrays
void Sprite::SetupSprite(float vertices[], unsigned int indices[])
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(float), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(float), indices, GL_STATIC_DRAW);

    // Position attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texture coordinates attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); // Unbind
}

// Make the OpenGL draw call
void Sprite::Draw()
{
    glActiveTexture(GL_TEXTURE0); // Associate the texture to GL_TEXTURE0 texture unit
    glBindTexture(GL_TEXTURE_2D, spriteTextureID);
    spriteShader->setInt("texture1", 0);
    
    glBindVertexArray(VAO); // Bind the vertex object
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0); // Once drawn we unbind the vertex object
    
    glBindTexture(GL_TEXTURE_2D, 0); // unbind texture

    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        LOG(ERROR, "OpenGL Sprite::Draw() Error: " << error);
    }
}
