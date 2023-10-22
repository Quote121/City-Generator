#include "sprite.hpp"
#include "stb_image/stb_image.h"

#include "config.hpp"

Sprite::Sprite(Shader* spriteShader_in, const std::string& spriteTexturePath_in, BoundingBox* boundingBox) : texturePath{spriteTexturePath_in}
{
    boundingBoxPtr = boundingBox;

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

    float x, y;

    unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    float fHeight, fWidth;
    fHeight = static_cast<float>(height);
    fWidth = static_cast<float>(width);

    // here we get the ratio of width and height of the texture
    // so the texture isnt strange
    if (width > height)
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
    boundingBoxPtr->StreamVertexUpdate(x, y, 0);
    boundingBoxPtr->StreamVertexUpdate(x, -y, 0);
    boundingBoxPtr->StreamVertexUpdate(-x, -y, 0);
    boundingBoxPtr->StreamVertexUpdate(-x, y, 0);


    // textureID to be stored
    spriteTextureID = textureID;
    // shader to be stored
    spriteShader = spriteShader_in;

    SetupSprite(vertices, indices);
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


// Sprite Shader getters and setters

// Shader* Sprite::GetSpriteShader() 
    

