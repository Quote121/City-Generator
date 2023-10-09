#include "sprite.hpp"
#include "stb_image/stb_image.h"

Sprite::Sprite(Shader* spriteShader_in, const std::string& spriteTexturePath_in) : texturePath{spriteTexturePath_in}
{
    

    std::string filename = std::string(spriteTexturePath_in);
    
    // filename = directory + '/' + filename; // assume relative

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        // Channels in file then we set the format
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        else {}
            //TODO | log statement for what component is

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << spriteTexturePath_in << std::endl;
    }
    stbi_image_free(data);

    // // 
    // float vertices[] = {
    // // positions           // texture coords
    //      0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   // top right
    //      0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // bottom right
    //     -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // bottom left
    //     -0.5f,  0.5f, 0.0f,   0.0f, 1.0f    // top left 
    // };

    float x, y;
    float vertices[] = {
    // positions           // texture coords
         x,  y, 0.0f,   1.0f, 1.0f,   // top right
         x, -y, 0.0f,   1.0f, 0.0f,   // bottom right
        -x, -y, 0.0f,   0.0f, 0.0f,   // bottom left
        -x,  y, 0.0f,   0.0f, 1.0f    // top left 
    };

    // here we get the ratio of width and height of the texture
    // so the texture isnt strange
    if (width > height)
    {
        x = 1;
        y = (height/width);
    }
    else if (height > width)
    {
        x = (width/height);
        y = 1;
    }
    else // width == height
    {
        x = 1; y = 1;
    }

    // textureID to be stored
    spriteTextureID = textureID;
    // shader to be stored
    spriteShader = spriteShader_in;

    SetupSprite(vertices);

}

// Setup vertex and buffer arrays
void Sprite::SetupSprite(float vertices[])
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texture coordinates attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

// Make the OpenGL draw call
void Sprite::Draw()
{
    // Here we do the openGL draw calls
    glBindTexture(GL_TEXTURE_2D, spriteTextureID);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}


// Sprite Shader getters and setters
void Sprite::SetSpriteShader(Shader* spriteShader_in) 
    { spriteShader = spriteShader_in; }
Shader* Sprite::GetSpriteShader() 
    { return spriteShader; }

