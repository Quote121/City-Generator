#include <skybox.hpp>

#include <config.hpp>
#include <shader.hpp>
#include <glad/glad.h>

SkyBox::SkyBox(std::array<std::string_view, 6> const& textureFaces_in, std::string const& alias)
{
    this->alias = alias;
    textureFaces = textureFaces_in;
    cubemapTextureid = loadCubeMap();
    SetupVertices();
    // load skybox default shader
    skyBoxShader = new Shader(paths::skybox_defaultVertShaderPath, paths::skybox_defaultFragShaderPath);
}


// GlDelete
SkyBox::~SkyBox()
{
    delete(skyBoxShader);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}


// Load all 6 cubemap texture into one texture
unsigned int SkyBox::loadCubeMap()
{
    // Front, back, up, down, right, left

    // Check we  have 6 faces
    if (textureFaces.size() != 6)
    {
        LOG(ERROR, "Cube map texture does not have 6 faces.");
        return 0; // Return none texture int
    }

    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < textureFaces.size(); i++)
    {
        stbi_set_flip_vertically_on_load(false);
        unsigned char *data = stbi_load(textureFaces[i].data(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                        0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
        }
        else
        {
            LOG(ERROR, "Cubemap texture failed to load at path: " << textureFaces[i]);
        }
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

void SkyBox::SetupVertices()
{
    float skyboxVertices[] = {
        // positions for 2 triangles     
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(float), skyboxVertices, GL_STATIC_DRAW);

    // Apos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void SkyBox::Draw(glm::mat4 view, glm::mat4 projection)
{
    glDepthMask(GL_FALSE);

    skyBoxShader->use();
    skyBoxShader->setMat4("view", view);
    skyBoxShader->setMat4("projection", projection);
    
    glActiveTexture(GL_TEXTURE0);       // Activate texture unit, bind the texture id
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTextureid);
    skyBoxShader->setInt("skybox", 0);  // Set the texture unit in the shader

    glBindVertexArray(VAO);             // Draw
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);

    glBindTexture(GL_TEXTURE_2D, 0); // unbind texture
}
