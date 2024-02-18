#pragma once

#include <glm/glm.hpp>
#include <vertexArray.hpp>
#include <vertexBuffer.hpp>

// Forward declaration
class Shader;

class Line
{
private:
    VertexArray* VAO;
    VertexBuffer* VBO;

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
