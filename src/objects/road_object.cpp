#include "road_object.hpp"
#include "resourceManager.hpp"
#include "camera.hpp"
#include "scene.hpp"
#include <road_zone_object.hpp>

RoadObject::RoadObject(const glm::vec3 point_a,
                       const glm::vec3 point_b,
                       const float roadWidth_in,
                       Shader* shader_in) : roadPointA(point_a), roadPointB(point_b), roadWidth(roadWidth_in) 
{
    Shader* shader;

    if (shader_in == nullptr)
    {
        shader = ResourceManager::getInstance()->LoadShader(paths::road_defaultVertShaderPath, paths::road_defaultFragShaderPath);
    }
    else
    {
        shader = shader_in;
    }
    road_renderer = new Road(shader);
   
    // Call master updater
    UpdateRoad(roadPointA, roadPointB); 
}

RoadObject::~RoadObject()
{
    delete(road_renderer);
    delete(zoneA);
    delete(zoneB);
}

void RoadObject::Draw(glm::mat4 view, glm::mat4 projection)
{
    // Set all shader properties in here

    glm::mat4 result = glm::mat4(1.0f);
    Shader* objectShader = road_renderer->GetRoadShader();

    objectShader->use();
    objectShader->setMat4("view", view);
    objectShader->setMat4("projection", projection);
    objectShader->setMat4("model", result);
    objectShader->setVec3("colour", roadColour); 

    objectShader->setBool("ShowLighting", enableLighting);

    // Lighting
    if (enableLighting)
    {
        // TEMP we go through all point lights and assign the values form it here
        objectShader->setVec3("viewPos", Camera::getInstance()->Position);
        objectShader->setFloat("material.shininess", 5.0f);
        
        size_t pointLightSize = Scene::getInstance()->GetPointLightObjects().size();
        
        objectShader->setInt("NumValidPointLights", pointLightSize);

        // For each point light set the corresponding values
        for (long unsigned int i = 0; i < pointLightSize; i++)
        {
            auto& light = Scene::getInstance()->GetPointLightObjects().at(i);
            std::string lightName = "pointLights[" + std::to_string(i) + "]";
            
            objectShader->setVec3((lightName + ".position"), light->GetPosition());
            objectShader->setVec3((lightName + ".ambient"), light->GetAmbient());
            objectShader->setVec3((lightName + ".diffuse"), light->GetDiffuse());
            objectShader->setVec3((lightName + ".specular"), light->GetSpecular());
            objectShader->setFloat((lightName + ".constant"), light->GetConstant());
            objectShader->setFloat((lightName + ".linear"), light->GetLinear());
            objectShader->setFloat((lightName + ".quadratic"), light->GetQuadratic());
        }

        // Directional lights
        DirectionalLightObject* dirLight = Scene::getInstance()->GetDirectionalLightObjects().at(0);
        objectShader->setVec3(("dirLight.direction"), dirLight->GetDirection());

        objectShader->setVec3(("dirLight.ambient"), dirLight->GetAmbient());
        objectShader->setVec3(("dirLight.diffuse"), dirLight->GetDiffuse());
        objectShader->setVec3(("dirLight.specular"), dirLight->GetSpecular());
    }

    // Call underlying road renderer
    road_renderer->Draw();

    if (showZones)
    {
        zoneA->Draw(view, projection);
        zoneB->Draw(view, projection);
    }

}


void RoadObject::UpdateRoad(const glm::vec3 a, const glm::vec3 b)
{
    // Update renderer vertices
    road_renderer->UpdateVertices(a, b, roadWidth);
    // After road renderer has updated it will update zone and bounding box data
    roadBBPoints = road_renderer->getOBB();

    // Check if either road object is initalized if so then initalize, otherwise update
    if (zoneA == nullptr) zoneA = new RoadZoneObject(road_renderer->road_left_zone_vertices, roadWidth);
    else zoneA->UpdateVertices(road_renderer->road_left_zone_vertices, roadWidth); 
    if (zoneB == nullptr) zoneB = new RoadZoneObject(road_renderer->road_right_zone_vertices, roadWidth);
    else zoneB->UpdateVertices(road_renderer->road_right_zone_vertices, roadWidth); 
}




bool RoadObject::TooFarForCollision(const RoadObject* road, const float threshold)
{
    // Get radius of both roads from center to edge and then the threshold is the gap between the two radii
    // Our radius
    glm::vec3 ourCenter = roadBBPoints[0] + ((roadBBPoints[1] - roadBBPoints[0])/2.0f) + ((roadBBPoints[2] - roadBBPoints[1])/2.0f);
    float ourRadius = glm::length(ourCenter-roadBBPoints[0]);

    // Their radius
    auto roadPoints = road->GetRoadOBB();
    glm::vec3 theirCenter = roadPoints[0] + ((roadPoints[1] - roadPoints[0])/2.0f) + ((roadPoints[2] - roadPoints[1])/2.0f);
    float theirRadius = glm::length(theirCenter-roadPoints[0]);

    // If the distance between the two radiuss of each road is above the thresold then we are good
    return (glm::length(ourCenter-theirCenter) - ourRadius - theirRadius) > threshold;
}


// Require a vertice update for the builders
// Builders
RoadObject* RoadObject::SetWidth(float width_in)
{
    if (width_in < 0.1)
    {
        roadWidth = 0.1;
    }
    else {
        roadWidth = width_in;
    }
    road_renderer->UpdateVertices(roadPointA, roadPointB, roadWidth);
    return this;
}

// This could be removed later on as we implement LOD
RoadObject* RoadObject::SetCurveSides(unsigned int sides)
{
    road_renderer->SetRoadCurveSides(sides);
    return this;
}

RoadObject* RoadObject::SetColour(glm::vec3 colour)
{
    roadColour = colour;
    return this;
}

// ImGui
float& RoadObject::GetWidthImGui(void)
{
    return roadWidth;
}

unsigned int& RoadObject::GetCurveSidesImGui(void)
{
    return road_renderer->roadCurveSides;
}

