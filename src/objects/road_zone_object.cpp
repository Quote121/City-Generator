#include <road_zone_object.hpp>
#include <glm/glm.hpp>
#include <resourceManager.hpp>

RoadZoneObject::RoadZoneObject(std::array<glm::vec3, 4>& vertices_in, float width_in) : vertices{vertices_in}, roadWidth{width_in}
{
    // Load line shader
    zoneShader = ResourceManager::getInstance()->LoadShader(paths::line_defaultVertShaderPath, paths::line_defaultFragShaderPath);

    // Create zone_renderer
    zone_renderer = new Zone(vertices, roadWidth);
}

RoadZoneObject::~RoadZoneObject()
{
    delete(zone_renderer);
}

void RoadZoneObject::Draw(glm::mat4 view, glm::mat4 projection)
{
    // Setup shader attributes
    
    
    glm::mat4 result = glm::mat4(1.0f);

    zoneShader->use();
    zoneShader->setMat4("view", view);
    zoneShader->setMat4("projection", projection);
    zoneShader->setMat4("model", result);
    zoneShader->setVec3("colour", zoneColour); 
    
    zone_renderer->Draw();
}

