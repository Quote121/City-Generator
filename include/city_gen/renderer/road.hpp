#pragma once

#include <glm/glm.hpp>
#include <bounding_box.hpp>

// Forward declaration
class Shader;

class Road
{
private:
    unsigned int VAO, VBO;
    // as to be multiple of 4, at least 4 (Will createa point at the end of the road at 4)
    // TODO In future this could be set to ranges based on level of detail (close more vertices, further the less)
    unsigned int roadCurveSides = 40; 
    // unsigned int roadCurveSides = 4;//0;     

    Shader* roadShader;

    BoundingBox* road_bb;

    // Private as we want certain road object values to be updated before
    // Pass both point and road width to recalculate the vertices of the road
    void UpdateVertices(glm::vec3 point_a, glm::vec3 point_b, float width);

public:
    // So that we can access the private values without extra getters for ImGui
    friend class RoadObject;

       // Make the opengl draw calls
    void Draw(glm::mat4 view, glm::mat4 projection);

    Road(Shader* shader);
    ~Road();

    BoundingBox* GetBoundingBox(void)
    {
        return road_bb;
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
