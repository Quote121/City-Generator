#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp> // opengl mathematics
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>

#include "shader.hpp"

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

class Mesh{

    private:
        // Render data
        unsigned int VAO, VBO, EBO;

        void setupMesh();

    public:
        // Mesh data
        std::vector<Vertex>         vertices;
        std::vector<unsigned int>   indices;
        std::vector<Texture>        textures;

        Mesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, std::vector<Texture> textures);
        void Draw(Shader &shader);


};

#endif // MESH_H