#pragma once

#include <zone.hpp>
#include <config.hpp>
#include <vector>

// Forward declaration
class Shader;

class RoadZoneObject
{
private:
    Zone* zone_renderer;
    Shader* zoneShader;

    glm::vec3 zoneColour = GREEN;

    std::array<glm::vec3, 4> vertices;
   
    // Determine width and height for bounding box collisions
    // Bouding box collision are fast so we can test that first and then cull cells
    // After cells have been culled we have another meathod that determines the largest area for object placement
    
    // Some way of storing each cell and its position for lookup later
    // Width and height of cell (density), size of the cells should scale with width

    float roadWidth;

public:

    RoadZoneObject(std::array<glm::vec3, 4>& vertices_in, float width_in);

    ~RoadZoneObject();

    void UpdateVertices(const std::array<glm::vec3, 4>& vertices_in, const float width_in);
    
    // Pass in the 4 points of the bounding box, we determine 
    bool Intersects(const std::array<glm::vec3, 4>& boundingBox) const;

    // Update grid cell based on the collision
    void UpdateCells(void);

    // Returns the largest area to place an object
    void LargestAreaForPlacement(void);

    void SetColour(glm::vec3 colour);

    void Draw(glm::mat4 view, glm::mat4 projection);
};

