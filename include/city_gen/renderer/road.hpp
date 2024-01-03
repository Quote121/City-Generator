#pragma once

#include <glm/glm.hpp>

// Forward declaration
class Shader;

class Road
{
private:
    unsigned int VAO, VBO;
    unsigned int roadCurveSides = 40; // This value will be times by 4 within the draw method
    float width = 3.0f;

    Shader* roadShader;
public:
    // So that we can access the private values without extra getters for ImGui
    friend class RoadObject;

    void UpdateVertices(glm::vec3 point_a, glm::vec3 point_b);

    // Make the opengl draw calls
    void Draw(glm::mat4 view, glm::mat4 projection);

    Road(Shader* shader);
    ~Road();


    // Getters and setters
    void SetRoadWidth(float width_in)
    {
        if (width_in > 0)
        {
            width = width_in;
        }
        else
        {
            width = 0.1f;
        }
    }

    inline float GetRoadWidth(void)
    {
        return width;
    }


    void SetRoadCurveSides(unsigned int sides)
    {
        if (sides > 0)
        {
            roadCurveSides = sides;
        }
        else
        {
            roadCurveSides = 1;
        }
    }

    inline unsigned int GetRoadCurveSides()
    {
        return roadCurveSides;
    }


    inline Shader* GetRoadShader()
    {
        return roadShader;
    }

    // ImGui Handles
    // If we change these values at runtime we will need to call update vertices too so that the
    // road will update with the new parameters

    // We should check if the value has changed, if not no need to update. An update will be expensive

    

};
