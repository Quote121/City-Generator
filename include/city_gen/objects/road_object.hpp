#pragma once

#include <base_object.hpp>
// #include <shader.hpp>
#include <road.hpp> 

// Line object
class RoadObject
{
private:

    // The two vertices that describe the line
    glm::vec3 a; 
    glm::vec3 b;

    Road* road_obj;

    std::string alias;
    bool enableLighting = true;

public:
    RoadObject(glm::vec3 point_a,
               glm::vec3 point_b,
               Shader* shader_in);

    ~RoadObject();

    void Draw(glm::mat4 view, glm::mat4 projection);

    // Builders for width and road curve vertices
    // an update will be needed for this
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

};
