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
    // Make sure we dont pass a nullptr to the modelObject constructor
    assert(shader_in != nullptr);
    model = ResourceManager::getInstance()->LoadModel(modelPath_in, shader_in);
}

ModelObject::~ModelObject() {}


void ModelObject::Draw(glm::mat4 view, glm::mat4 projection)
{
    if (isVisible)
    {
        glm::mat4 result = this->GetModelMatrix(); 
        Shader* objectShader = model->GetShader();

        if (objectShader == nullptr)
        {
            LOG(ERROR, "NO SHADER LOADED TO OBJECT CLASS");
        }
        else
        {
            // Apply all position and scaling before drawing
            objectShader->use();
            objectShader->setMat4("view", view);
            objectShader->setMat4("projection", projection);
            objectShader->setMat4("model", result);
            // Set the local position based on the bounding box center

            objectShader->setVec2("textureScale", textureScale);
            // Tells the shader wether to show the lighting or just the base ambient texture
            objectShader->setBool("ShowLighting", lightingEnable);
            // Lighting
            if (lightingEnable)
            {
                Scene::getInstance()->SetShaderLights(objectShader); 
            }
            model->Model::Draw();
        }
        // Draw bounding box if asked
        if (showBoundingBox)
        {
            this->DrawBoundingBox(WHITE);
        }
    }
}


void ModelObject::DrawInstances(glm::mat4 view, glm::mat4 projection, std::vector<float>* matrices)
{
    if (isVisible)
    {
        Shader* objectShader = model->GetShader();

        // We will want to instance draw other things
        // Shader* objectShader = ResourceManager::getInstance()->LoadShader(paths::building_defaultInstancedVertShaderPath, paths::building_defaultFragShaderPath);
        if (objectShader == nullptr)
        {
            LOG(ERROR, "NO SHADER LOADED TO OBJECT CLASS");
        }
        else
        {
            // Apply all position and scaling before drawing
            objectShader->use();
            objectShader->setMat4("view", view);
            objectShader->setMat4("projection", projection);
       
            // Tells the shader wether to show the lighting or just the base ambient texture
            objectShader->setBool("ShowLighting", lightingEnable);
            // Lighting
            if (lightingEnable)
            {
                Scene::getInstance()->SetShaderLights(objectShader);
            }
            model->Model::DrawInstanced(matrices);
            // model->Model::Draw();
        }
    }
}


void ModelObject::DrawBoundingBox(glm::vec3 colour)
{
    glm::mat4 view = Camera::getInstance()->GetViewMatrix();
    glm::mat4 projection = Camera::getInstance()->GetProjectionMatrix();
    BoundingBox* bb = this->model->GetBoundingBox();
    Shader* bbShader = bb->getShader();
    bbShader->use();
    bbShader->setMat4("view", view);
    bbShader->setMat4("projection", projection);
    bbShader->setMat4("model", this->GetModelMatrix());
    bbShader->setVec3("colour", colour);

    bb->Draw();
}


glm::mat4 ModelObject::GetModelMatrix(void)
{
    return glm::mat4(1.0f) * 
        getPositionMat4(position) * 
        getRotateMat4(rotation) * 
        getScaleMat4(scaleScalar) * 
        getScaleMat4(scale) / 
        getPositionMat4(objectOriginPosition); // To set the models origin
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

ModelObject* ModelObject::IsSelectable(bool toggle)
{
    isSelectable = toggle;
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


ModelObject* ModelObject::SetInstaceRendering(bool toggle)
{
    instanceRender = toggle;
    return this;
}


ModelObject* ModelObject::SetOriginFrontLeft()
{
    objectOriginPosition = model->GetBoundingBox()->getFrontLeftBuilding();
    return this;
}

ModelObject* ModelObject::SetOriginFrontRight()
{
    objectOriginPosition = model->GetBoundingBox()->getFrontRightBuilding();
    return this;
}


ModelObject* ModelObject::SetTextureScale(glm::vec2 scale)
{
    textureScale = scale;
    return this;
}

// Getters and setters
glm::vec3 ModelObject::GetScale() const
{
    return this->scale;
}

std::string ModelObject::GetModelName() const
{
    return model->GetModelName();
}

std::string ModelObject::GetModelPath() const
{
    return model->GetModelPath();
}

bool ModelObject::GetShowBoundingBox() const
{
    return showBoundingBox;
}


bool ModelObject::GetIsInstanceRendered(void) const
{
    return instanceRender;
}


BoundingBox* ModelObject::GetBoundingBox(void) const
{
    return model->GetBoundingBox();
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
