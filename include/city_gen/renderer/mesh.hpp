#pragma once

#include <glm/glm.hpp> // opengl mathematics
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>

// Forward declaration
class Shader;

struct Vertex{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoord;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

// Not for textures, just raw values
struct Material {
    glm::vec3 ambience = {0.5f, 0.5f, 0.5f};
    glm::vec3 diffuse = {1.0f, 1.0f, 1.0f};
    glm::vec3 specular = {0.4f, 0.4f, 0.4f};
    float shininess = 20.0f;
};

class Mesh{

private:
    // Render data
    unsigned int VAO, VBO, EBO;

    // Buffer for matrix data in instancing
    unsigned int matrixBuffer;

    void setupMesh();

public:
    // Mesh data
    std::vector<Vertex>         vertices;
    std::vector<unsigned int>   indices;
    std::vector<Texture>        textures;
    Material                    material;

    Mesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, std::vector<Texture> textures, Material material);
    ~Mesh();
    void Draw(Shader &shader);
    void DrawInstanced(Shader &shader, unsigned int instanceCount, std::vector<float>* matrices);
};

