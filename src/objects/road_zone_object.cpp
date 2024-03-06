#include "scene.hpp"
#include <road_zone_object.hpp>
#include <glm/glm.hpp>
#include <resourceManager.hpp>
#include <helper.hpp>

RoadZoneObject::RoadZoneObject(std::array<glm::vec3, 4>& vertices_in, float width_in) : vertices{vertices_in}, roadWidth{width_in}
{
    // Load line shader
    zoneShader = ResourceManager::getInstance()->LoadShader(paths::line_defaultVertShaderPath, paths::line_defaultFragShaderPath);

    // Update the vertices
    this->UpdateVertices(vertices, roadWidth);
}

RoadZoneObject::~RoadZoneObject()
{
    delete(zone_renderer);
}

// Update vertices of zone
void RoadZoneObject::UpdateVertices(const std::array<glm::vec3, 4>& vertices_in, const float width_in)
{
    if (zone_renderer == nullptr)
    {
        // Create zone_renderer
        zone_renderer = new Zone(vertices, roadWidth);
    } 

    vertices = vertices_in;
    zone_renderer->UpdateVertices(vertices_in, width_in);

    // Determine how many zones it has
    float length = glm::length(vertices_in[0] - vertices_in[1]);
    int sectionCount = glm::floor(length/width_in);

    glm::vec3 newVec = (vertices_in[0] - vertices_in[1]);
    glm::vec3 unitVector = newVec /= sectionCount;

    float flip = 0;
    if (vertices[0].x > vertices[1].x) { flip = M_PI; }

    for (int i = 0; i < sectionCount; i++)
    {
        glm::vec3 x = (unitVector * glm::vec3(i));
        glm::vec3 placementVector = vertices_in[0] - x;

        glm::vec3 invUnitVector = {-unitVector.z, unitVector.y, unitVector.x};

        // Zone vertices
        glm::vec3 one = placementVector;
        glm::vec3 two = placementVector-unitVector;
        glm::vec3 three = placementVector-invUnitVector;
        glm::vec3 four = placementVector-unitVector-invUnitVector;

        areasForPlacement.push_back({placementVector, false, 
                                     this->GetZoneAngle()+flip, {one, two, four, three}});
    }
}


PlacementArea const* RoadZoneObject::GetValidPlacement(void)
{
    for (unsigned int i = 0; i < areasForPlacement.size(); i++)
    {
        if (!areasForPlacement[i].isOccupied)
        {
            areasForPlacement[i].isOccupied = true;
            return &areasForPlacement[i]; // return copy
        }
    }
    return nullptr;
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
        if (!projectionOverlap(vertices, boundingBox, axis[i]))
        {
            return false;
        }
    }

    // Does not intersect
    return true;
}

void RoadZoneObject::SetColour(glm::vec3 colour)
{
    zoneColour = colour;
}


void RoadZoneObject::SetZoneUsable(bool toggle)
{
    useZone = toggle;
}


bool RoadZoneObject::IsUsable(void) const
{
    return useZone;
}


float RoadZoneObject::GetZoneAngle(void)
{    
    return glm::atan((vertices[1].z - vertices[0].z)/(vertices[1].x - vertices[0].x));
}


void RoadZoneObject::Draw(glm::mat4 view, glm::mat4 projection)
{
    // If the road is green or should be shown based on intersections
    if (zoneColour != RED || !Scene::getInstance()->GetRemoveIntersectingZones())
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
}

