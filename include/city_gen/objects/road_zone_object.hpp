#pragma once

#include <zone.hpp>
#include <config.hpp>

// Forward declaration
class Shader;

class RoadZoneObject
{
private:
    Zone* zone_renderer;
    Shader* zoneShader;

    glm::vec3 zoneColour = GREEN;

    std::array<glm::vec3, 4> vertices;
    float roadWidth;

public:

    RoadZoneObject(std::array<glm::vec3, 4>& vertices_in, float width_in);

    ~RoadZoneObject();
    
    void Draw(glm::mat4 view, glm::mat4 projection);
};

