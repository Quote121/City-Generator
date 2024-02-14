#include "glm/exponential.hpp"
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

// Update vertices of zone
void RoadZoneObject::UpdateVertices(const std::array<glm::vec3, 4>& vertices_in, const float width_in)
{
    vertices = vertices_in;
    zone_renderer->UpdateVertices(vertices_in, width_in);
}

// helper function
bool projectionOverlap(const std::array<glm::vec3, 4>& a, const std::array<glm::vec3, 4>& b, const glm::vec3 axis)
{
    std::array<float, 4> projections_a, projections_b;
    float min_a, max_a, min_b, max_b;

    int i;

    // Project vertices onto axis
    for (i = 0; i < 4; i++)
    {
        projections_a[i] = glm::dot(a[i], axis);
        projections_b[i] = glm::dot(b[i], axis);
    }

    // Initalize the min and maxes
    min_a = projections_a[0];
    max_a = projections_a[0];
    min_b = projections_b[0];
    max_b = projections_b[0];

    for (i = 0; i < 4; i++)
    {
        if (projections_a[i] < min_a) min_a = projections_a[i];
        if (projections_a[i] > max_a) max_a = projections_a[i];
        if (projections_b[i] < min_b) min_b = projections_b[i];
        if (projections_b[i] > max_b) max_b = projections_b[i];
    }

    if (max_a < min_b || max_b < min_a)
    {
        // No overlap
        return false;
    }
    // Overlap
    return true;
}

// Get perpendicular edges
inline glm::vec3 getPerpendicularXZ(glm::vec3 vector)
{
    glm::vec3 returnVector;
    returnVector.x = -vector.z;
    returnVector.y = vector.y;
    returnVector.z = vector.x;
    return returnVector;
}

// Checks if two orienteted rectangles collide, using SAT - seperate axis theorem
bool RoadZoneObject::Intersects(const std::array<glm::vec3, 4>& boundingBox) const
{
    // First we check the axis of our zone
    std::array<glm::vec3, 8> axis;

    size_t i;
    // Get all 4 edges of both rectangles
    for (i = 0; i < 4; i++)
    {
        // Get each vector of each edge and then get its perits adjacent one and then put them into a bix array
        axis[i] = glm::normalize(getPerpendicularXZ(vertices[(i + 1) % 4] - vertices[i]));
        axis[i+4] = glm::normalize(getPerpendicularXZ(boundingBox[(i + 1) % 4] - boundingBox[i]));
    }

    for (i = 0; i < 8; i++)
    {
        if (projectionOverlap(vertices, boundingBox, axis[i]))
        {
            return true;
        }
    }

    // Does not intersect
    return false;
}

void RoadZoneObject::SetColour(glm::vec3 colour)
{
    zoneColour = colour;
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

