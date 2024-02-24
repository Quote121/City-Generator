#pragma once
#include <array>
#include <glm/glm.hpp>

#include <vertexArray.hpp>
#include <vertexBuffer.hpp>
#include <indexBuffer.hpp>

// Zone renderer for debugging

class Zone
{
private:
    VertexArray* VAO;
    VertexBuffer* VBO;

public:

    Zone(const std::array<glm::vec3,4>& array, const float width);
    
    ~Zone();
    
    // Method that updates and draws the rectangle for the zone
    void UpdateVertices(const std::array<glm::vec3, 4>& array, const float width);
    
    void Draw();
};

