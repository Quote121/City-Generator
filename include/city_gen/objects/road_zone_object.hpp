#pragma once

#include <zone.hpp>
#include <config.hpp>


struct PlacementArea
{
    glm::vec3 position;
    bool isOccupied = false;
    float angle; // direction based on 0 being -Z
};

// Forward declaration
class Shader;

class RoadZoneObject
{
private:
    
    // DEBUG RENDERER
    Zone* zone_renderer = nullptr;
    Shader* zoneShader;
    glm::vec3 zoneColour = GREEN;
    // DEBUG RENDERER
   

    // To say if we should use this zone
    bool useZone = true;

    std::array<glm::vec3, 4> vertices;

    // Vector of positions for road placement
    std::vector<PlacementArea> areasForPlacement;

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

    void SetZoneUsable(bool toggle);
    bool IsUsable(void) const;

    // return a valid placement area, the method will set the use as true and then return position information
    // @return a const* PlacementArea. If no valid areas returns nullptr
    PlacementArea const* GetValidPlacement(void);

    // Get the angle of the zoneShade
    float GetZoneAngle(void);

    

    void Draw(glm::mat4 view, glm::mat4 projection);
};

