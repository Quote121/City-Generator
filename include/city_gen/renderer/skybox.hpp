#pragma once

#include <glad/glad.h>
#include <stb_image/stb_image.h>
#include <glm/glm.hpp>

#include <vector>
#include <string>

// Forward declaration
class Shader;

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
    SkyBox(std::vector<std::string> textureFaces_in);
    ~SkyBox();
    void Draw(glm::mat4 view, glm::mat4 projection);
};
