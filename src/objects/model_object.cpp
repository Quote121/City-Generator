#include <model_object.hpp>

#include <config.hpp>
#include <shader.hpp>
#include <resourceManager.hpp>
#include <bounding_box.hpp>

ModelObject::ModelObject(std::string& modelPath_in,   // Path to .obj
                         Shader *shader_in) :                        
                         BaseObject()
{
    Shader* shader;
    // Get the model name
    modelName = modelPath_in.substr(modelPath_in.find_last_of('/')+1, modelPath_in.size()-1);

    // Create the default shader unless passed one
    if (shader_in == nullptr)
    {
        LOG(WARN, "Object shader null -- Setting default shader");
        // Create heap object shader
        shader = ResourceManager::getInstance()->LoadShader(paths::object_defaultVertShaderPath, paths::object_defaultFragShaderPath);
    }
    else
    {
        shader = shader_in;
    }
    model = new Model(shader, modelPath_in, base_boundingBox);
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
            model->Model::Draw();
        }

        // Draw bounding box if asked
        if (showBoundingBox)
        {
            Shader* bbShader = base_boundingBox->getShader();
            bbShader->use();
            bbShader->setMat4("view", view);
            bbShader->setMat4("projection", projection);
            bbShader->setMat4("model", result);
            bbShader->setVec3("localCenterPos", objectOriginPosition);

            base_boundingBox->Draw();
        }
    }
}






// Builders
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
    glm::vec3 center = base_boundingBox->getCenter();
    center.y = base_boundingBox->getMin().y;
    objectOriginPosition = center;
    return this;
}

ModelObject* ModelObject::SetModelOriginCenter()
{
    objectOriginPosition = base_boundingBox->getCenter();
    return this;
}

std::string const& ModelObject::GetModelName()
{
    return modelName;
}

// ImGui
glm::vec3& ModelObject::GetScaleImGui()
{
    return scale;
}