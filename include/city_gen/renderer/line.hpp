#pragma once
#include <glm/glm.hpp>

// Forward declaration
class Shader;

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
    ~Line();

    inline Shader* GetLineShader()
    {
        return lineShader;
    }
};
