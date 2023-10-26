#pragma once
#include <glm/glm.hpp>
#include "shader.hpp"
#include "config.hpp"

class Line
{
private:
    unsigned int VAO, VBO;
    Shader* lineShader;

public:

    // Will rebind VAO for the new vertex positions
    void UpdateVerts(glm::vec3 point_a, glm::vec3 point_b);
    
    // Make the opengl draw calls
    void Draw();

    Line(Shader* shader);

    ~Line()
    {
        delete(lineShader);
        glDeleteBuffers(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    inline Shader* GetLineShader()
    {
        return lineShader;
    }
};
