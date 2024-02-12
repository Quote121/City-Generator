#pragma once

#include <road.hpp> 
#include <config.hpp>
#include <road_zone_object.hpp>

// Line object
class RoadObject
{
private:

    // The two vertices that describe the line
    glm::vec3 a; 
    glm::vec3 b;
 
    // Width (diameter of the road)
    float roadWidth;

    // Road zones
    RoadZoneObject* leftZone;
    RoadZoneObject* rightZone;

    // Underlying rendering object
    Road* road_obj;

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

    void Draw(glm::mat4 view, glm::mat4 projection);

    // Builders for width and road curve vertices
    RoadObject* SetWidth(float width);               // Calls a vertice update
    RoadObject* SetCurveSides(unsigned int sides);   
    RoadObject* SetColour(glm::vec3 colour);

    // ImGui
    float& GetWidthImGui(void);
    unsigned int& GetCurveSidesImGui(void);

    // Other getters and setters
    void SetAlias(std::string& name)
    {
        alias = name;
    }

    std::string GetAlias(void) const
    {
        return alias;
    }

    glm::vec3 getPointA(void) const
    {
        return a; 
    }

    glm::vec3 getPointB(void) const
    {
        return b;
    }

   

};
