#pragma once

#include <zone.hpp>
#include <config.hpp>
#include <helper.hpp>

// helper function
// bool projectionOverlap(const std::array<glm::vec3, 4>& a, const std::array<glm::vec3, 4>& b, const glm::vec3 axis)
// {
//     std::array<float, 4> projections_a, projections_b;
//     float min_a, max_a, min_b, max_b;
//
//     int i;
//
//     // Project vertices onto axis
//     for (i = 0; i < 4; i++)
//     {
//         projections_a[i] = glm::dot(a[i], axis);
//         projections_b[i] = glm::dot(b[i], axis);
//     }
//
//     // Initalize the min and maxes
//     min_a = projections_a[0];
//     max_a = projections_a[0];
//     min_b = projections_b[0];
//     max_b = projections_b[0];
//
//     for (i = 0; i < 4; i++)
//     {
//         if (projections_a[i] < min_a) min_a = projections_a[i];
//         if (projections_a[i] > max_a) max_a = projections_a[i];
//         if (projections_b[i] < min_b) min_b = projections_b[i];
//         if (projections_b[i] > max_b) max_b = projections_b[i];
//     }
//
//     constexpr float lenience = 0.05; 
//     if (max_a <= min_b + lenience || max_b <= min_a + lenience)
//     {
//         // No overlap
//         return false;
//     }
//     // Overlap
//     return true;
// }

// Get perpendicular edges
// inline glm::vec3 getPerpendicularXZ(glm::vec3 vector)
// {
//     glm::vec3 returnVector;
//     returnVector.x = -vector.z;
//     returnVector.y = vector.y;
//     returnVector.z = vector.x;
//     return returnVector;
// }


constexpr float buildingCollisionThresholdDetection = 2.82842712475; // sqrt 8
struct PlacementArea
{
    glm::vec3 position;
    bool isOccupied = false;
    float angle; // direction based on 0 being -Z
    std::array<glm::vec3, 4> zoneVerticesArray; // For collision    

    // Checks if two orienteted rectangles collide, using SAT - seperate axis theorem
    bool Intersects(const std::array<glm::vec3, 4>& boundingBox)
    {
        // First we check the axis of our zone
        std::array<glm::vec3, 8> axis;

        size_t i;
        // Get all 4 edges of both rectangles
        for (i = 0; i < 4; i++)
        {
            // Get each vector of each edge and then get its perits adjacent one and then put them into a bix array
            axis[i] = glm::normalize(getPerpendicularXZ(zoneVerticesArray[(i + 1) % 4] - zoneVerticesArray[i]));
            axis[i+4] = glm::normalize(getPerpendicularXZ(boundingBox[(i + 1) % 4] - boundingBox[i]));
        }

        for (i = 0; i < 8; i++)
        {
            if (!projectionOverlap(zoneVerticesArray, boundingBox, axis[i]))
            {
                return false;
            }
        }

        // Does not intersect
        return true;
    }

    bool TooFarForCollision(const PlacementArea* area_in)
    {
        if (glm::length(position-area_in->position) <= buildingCollisionThresholdDetection)
        {
            return false;
        }
        return true;
    }

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

