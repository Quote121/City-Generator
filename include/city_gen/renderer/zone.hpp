#pragma once
#include <array>
#include <glm/glm.hpp>

// Zone renderer for debugging

class Zone
{
private:
    
    unsigned VAO, VBO;
    // unsigned EBO;

public:

    Zone(const std::array<glm::vec3,4>& array, const float width);
    
    ~Zone();
    
    // Method that updates and draws the rectangle for the zone
    void UpdateVertices(const std::array<glm::vec3, 4>& array, const float width);
    
    void Draw();
};

