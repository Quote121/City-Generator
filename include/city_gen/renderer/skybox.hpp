#pragma once

#include <glad/glad.h>
#include <stb_image/stb_image.h>

#include <vector>
#include <string>

#include "config.hpp"
#include "shader.hpp"

class SkyBox{
private:
    std::vector<std::string> textureFaces;
    unsigned int cubemapTextureid;
    unsigned int VAO, VBO;

    Shader* skyBoxShader;

    // Load all 6 cubemap texture into one texture
    unsigned int loadCubeMap();
    void SetupVertices();

public:
    SkyBox(std::vector<std::string> textureFaces_in)
    {
        textureFaces = textureFaces_in;
        cubemapTextureid = loadCubeMap();
        SetupVertices();
        // load skybox default shader
        skyBoxShader = new Shader(paths::skybox_defaultVertShaderPath, paths::skybox_defaultFragShaderPath);
    }

    // GlDelete
    ~SkyBox()
    {
        delete(skyBoxShader);
        glDeleteBuffers(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
    void Draw(glm::mat4 view, glm::mat4 projection);
};
