#include <model_object.hpp>

#include <config.hpp>
#include <shader.hpp>
#include <resourceManager.hpp>
#include <bounding_box.hpp>

// Lighting shader
#include <scene.hpp>
#include <camera.hpp>

ModelObject::ModelObject(const std::string& modelPath_in,   // Path to .obj
                         Shader *shader_in) :                        
                         BaseObject()
{
    Shader* shader;
    // Get the model name
    
    // Create the default shader unless passed one
    if (shader_in == nullptr)
    {
        // Create heap object shader
        shader = ResourceManager::getInstance()->LoadShader(paths::object_defaultVertShaderPath, paths::object_defaultFragShaderPath);
    }
    else
    {
        shader = shader_in;
    }

    model = ResourceManager::getInstance()->LoadModel(modelPath_in, shader);
}

ModelObject::~ModelObject()
{
    delete(model);
}


void ModelObject::Draw(glm::mat4 view, glm::mat4 projection)
{
    if (isVisible)
    {
        glm::mat4 result = glm::mat4(1.0f) * getPositionMat4(position) * getRotateMat4(rotation) * getScaleMat4(scaleScalar) * getScaleMat4(scale);
        Shader* objectShader = model->GetShader();

        if (objectShader == nullptr)
        {
            std::cerr << "NO SHADER LOADED TO OBJECT CLASS" << std::endl;
        }
        else
        {
            // Apply all position and scaling before drawing
            objectShader->use();
            objectShader->setMat4("view", view);
            objectShader->setMat4("projection", projection);
            objectShader->setMat4("model", result);
            // Set the local position based on the bounding box center
            objectShader->setVec3("localCenterPos", objectOriginPosition);
            
            // Tells the shader wether to show the lighting or just the base ambient texture
            objectShader->setBool("ShowLighting", lightingEnable);
            // Lighting
            if (lightingEnable)
            {
                // TEMP we go through all point lights and assign the values form it here
                objectShader->setVec3("viewPos", Camera::getInstance()->Position);
                objectShader->setFloat("material.shininess", 10.0f);
                
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
            model->Model::Draw();
        }

        // Draw bounding box if asked
        if (showBoundingBox)
        {
            BoundingBox* bb = model->GetBoundingBox();
            Shader* bbShader = bb->getShader();
            bbShader->use();
            bbShader->setMat4("view", view);
            bbShader->setMat4("projection", projection);
            bbShader->setMat4("model", result);
            bbShader->setVec3("localCenterPos", objectOriginPosition);
            bb->Draw();
        }
    }
}


// Model specific builders

ModelObject* ModelObject::SetPosition(glm::vec3 position_in)
{
    position = position_in;
    return this;
}

ModelObject* ModelObject::SetRotation(glm::vec3 rotation_in)
{
    rotation = rotation_in;
    return this;
}

ModelObject* ModelObject::SetScale(glm::vec3 scale_in)
{
    scale = scale_in;
    return this;
}

ModelObject* ModelObject::SetScale(float scale_in)
{
    scaleScalar = scale_in;
    return this;
}

ModelObject* ModelObject::IsVisible(bool toggle)
{
    isVisible = toggle;
    return this;
}

ModelObject* ModelObject::SetLightingEnabled(bool toggle)
{
    lightingEnable = toggle;
    return this;
}

ModelObject* ModelObject::ShowBoundingBox(bool toggle)
{
    showBoundingBox = toggle;
    return this;
}

ModelObject* ModelObject::SetSpawnOffset(glm::vec3 vec3)
{
    objectOriginPosition = vec3;
    return this;
}

ModelObject* ModelObject::SetModelOriginCenterBottom()
{
    glm::vec3 center = model->GetBoundingBox()->getCenter();
    center.y = model->GetBoundingBox()->getMin().y;
    objectOriginPosition = center;
    return this;
}

ModelObject* ModelObject::SetModelOriginCenter()
{
    objectOriginPosition = model->GetBoundingBox()->getCenter();
    return this;
}

// Getters and setters

std::string ModelObject::GetModelName() const
{
    return model->GetModelName();
}

bool ModelObject::GetShowBoundingBox() const
{
    return showBoundingBox;
}

// ImGui reference handles

glm::vec3& ModelObject::GetScaleImGui()
{
    return scale;
}

bool& ModelObject::GetShowBoundingBoxImGui()
{
    return showBoundingBox;
}

bool& ModelObject::GetShowLightingImGui()
{
    return lightingEnable;
}
