#include "road_object.hpp"
#include "resourceManager.hpp"
#include "camera.hpp"
#include "scene.hpp"

RoadObject::RoadObject(const glm::vec3 point_a,
                       const glm::vec3 point_b,
                       const float roadWidth_in,
                       Shader* shader_in) : a(point_a), b(point_b), roadWidth(roadWidth_in) 
{

    Shader* shader;

    if (shader_in == nullptr)
    {
        // shader = ResourceManager::getInstance()->LoadShader(paths::line_defaultVertShaderPath, paths::line_defaultFragShaderPath);
        shader = ResourceManager::getInstance()->LoadShader(paths::road_defaultVertShaderPath, paths::road_defaultFragShaderPath);
    }
    else
    {
        shader = shader_in;
    }
    road_obj = new Road(shader);
    // Initaliser list will be processed before constructor, but we pass these values
    road_obj->UpdateVertices(point_a, point_b, roadWidth_in);
}

RoadObject::~RoadObject()
{
    delete(road_obj);
}

void RoadObject::Draw(glm::mat4 view, glm::mat4 projection)
{
    // Set all shader properties in here

    glm::mat4 result = glm::mat4(1.0f);
    Shader* objectShader = road_obj->GetRoadShader();

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
    road_obj->Draw(view, projection);

    // Draw bouding box
    if (enableBoundingBox)
    {
        BoundingBox* bb = road_obj->GetBoundingBox();
        Shader* bbShader = bb->getShader();
        bbShader->use();
        bbShader->setMat4("view", view);
        bbShader->setMat4("projection", projection);
        bbShader->setMat4("model", result);
        bbShader->setVec3("localCenterPos", {0,0,0} );
        bb->Draw();
    }
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
    road_obj->UpdateVertices(a, b, roadWidth);
    return this;
}

// This could be removed later on as we implement LOD
RoadObject* RoadObject::SetCurveSides(unsigned int sides)
{
    road_obj->SetRoadCurveSides(sides);
    return this;
}

RoadObject* RoadObject::SetColour(glm::vec3 colour)
{
    roadColour = colour;
    return this;
}

RoadObject* RoadObject::SetBoundingBoxEnable(bool toggle)
{
    enableBoundingBox = toggle;
    return this;
}

// ImGui
float& RoadObject::GetWidthImGui(void)
{
    return roadWidth;
}

unsigned int& RoadObject::GetCurveSidesImGui(void)
{
    return road_obj->roadCurveSides;
}

