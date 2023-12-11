#pragma once

#include <base_object.hpp>
#include <road.hpp> 

// Line object
class RoadObject : public BaseObject<RoadObject>
{
private:

    // The two vertices that describe the line
    glm::vec3 a; 
    glm::vec3 b;

    Road* road_obj;

public:
    RoadObject(Shader* shader_in,
               glm::vec3 point_a,
               glm::vec3 point_b);

    ~RoadObject();

    void Draw(glm::mat4 view, glm::mat4 projection) override;

};
