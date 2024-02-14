#pragma once

#include <road.hpp> 
#include <config.hpp>
#include <road_zone_object.hpp>

// Line object
class RoadObject
{
private:

    // The two vertices that describe the line
    glm::vec3 roadPointA; 
    glm::vec3 roadPointB;
 
    // Width (diameter of the road)
    float roadWidth;

    // Road bounding box vertices
    std::array<glm::vec3, 4> roadBBPoints;

    // Road zones
    RoadZoneObject* zoneA = nullptr;
    RoadZoneObject* zoneB = nullptr;

    // Underlying rendering object
    Road* road_renderer;

    // Name for imgui
    std::string alias;
    glm::vec3 roadColour = DEFAULT_ROAD_COLOUR; // Light grey
    
    // Bool for rendering
    bool enableLighting = true;
    bool showZones = true;

public:
    RoadObject(const glm::vec3 point_a,
               const glm::vec3 point_b,
               const float roadWidth_in,
               Shader* shader_in);

    ~RoadObject();

    // This will take a threshold value and road object and determine if roads are far enough away to not do more costly collision calculations
    bool TooFarForCollision(const RoadObject* road, const float threshold);

    void Draw(glm::mat4 view, glm::mat4 projection);

    // Updates road zones and underlying road renderer
    void UpdateRoad(const glm::vec3 a, const glm::vec3 b);

    // Builders for width and road curve vertices
    RoadObject* SetWidth(float width);               // Calls a vertice update
    RoadObject* SetCurveSides(unsigned int sides);   
    RoadObject* SetColour(glm::vec3 colour);

    // ImGui
    float& GetWidthImGui(void);
    unsigned int& GetCurveSidesImGui(void);

    // Getters
    inline RoadZoneObject* GetZoneA(void) const
    {
        return zoneA;
    }

    inline RoadZoneObject* GetZoneB(void) const
    {
        return zoneB;
    }

    inline std::array<glm::vec3, 4> const& GetRoadOBB(void) const
    {
        return roadBBPoints;
    }


    // Called update instead of set as we update vertices too
    void UpdatePointA(const glm::vec3 a)
    {
        roadPointA = a;
        road_renderer->UpdateVertices(roadPointA, roadPointB, roadWidth);
    }

    void UpdatePointB(const glm::vec3 b)
    {
        roadPointB = b;
        road_renderer->UpdateVertices(roadPointA, roadPointB, roadWidth);
    }

    void UpdatePoints(const glm::vec3 a, const glm::vec3 b)
    {
        roadPointA = a; roadPointB = b;
        UpdateRoad(roadPointA, roadPointB);
    }

    // Other getters and setters
    void SetAlias(std::string& name)
    {
        alias = name;
    }


    std::string GetAlias(void) const
    {
        return alias;
    }

    glm::vec3 GetPointA(void) const
    {
        return roadPointA; 
    }

    glm::vec3 GetPointB(void) const
    {
        return roadPointB;
    }

    glm::vec3& GetPointAImGui(void)
    {
        return roadPointA;
    }

    glm::vec3& GetPointBImGui(void)
    {
        return roadPointB;
    }

   

};
