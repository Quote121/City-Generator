#pragma once

#include <glad/glad.h>
#include <stb_image/stb_image.h>
#include <glm/glm.hpp>

#include <string>
#include <array>

// Forward declaration
class Shader;

class SkyBox{
private:
    std::array<std::string_view, 6> textureFaces; 
    unsigned int cubemapTextureid;
    unsigned int VAO, VBO;

    std::string alias;
    Shader* skyBoxShader;

    // Load all 6 cubemap texture into one texture
    unsigned int loadCubeMap();
    void SetupVertices();

public:
    SkyBox(std::array<std::string_view, 6> const& textureFaces_in, std::string const& alias);
    ~SkyBox();
    void Draw(glm::mat4 view, glm::mat4 projection);

    std::string const& GetAlias(void) const
    {
        return this->alias;
    }
};
