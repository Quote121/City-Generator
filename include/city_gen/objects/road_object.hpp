#pragma once

#include <base_object.hpp>
// #include <shader.hpp>
#include <road.hpp> 
#include <sstream>

// Line object
class RoadObject
{
private:

    // The two vertices that describe the line
    glm::vec3 a; 
    glm::vec3 b;
    
    // Width (diameter of the road)
    float roadWidth;

    Road* road_obj;

    std::string alias;
    bool enableLighting = true;

public:
    RoadObject(const glm::vec3 point_a,
               const glm::vec3 point_b,
               const float roadWidth_in,
               Shader* shader_in);

    ~RoadObject();

    void Draw(glm::mat4 view, glm::mat4 projection);

    // Builders for width and road curve vertices
    // an update will be needed for this by calling updateVertices
    // this could be done (double overhead for road creation)
    // or could be added to constructor with a default value
    RoadObject* SetWidth(float width);
    RoadObject* SetCurveSides(unsigned int sides);

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


    // For debugging logs
    std::string GetPointAString(void) const
    {
        std::stringstream ss;
        ss << "[" << a.x << " " << a.y << " " << a.z << "]";
        return ss.str();
    }

    std::string GetPointBString(void) const
    {
        std::stringstream ss;
        ss << "[" << b.x << " " << b.y << " " << b.z << "]";
        return ss.str();
    }

};
