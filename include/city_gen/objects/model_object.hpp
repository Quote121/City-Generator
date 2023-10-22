#pragma once

#include "base_object.hpp"

#include "config.hpp"

// 3D object
class ModelObject : public BaseObject
{
private:
    glm::vec3 scale = {1.0f, 1.0f, 1.0f};
    Model* model;

    std::string modelName;

public:
    ModelObject(std::string& modelPath_in,   // Path to .obj
                Shader *shader_in) :                        
                BaseObject()
    {
        Shader* shader;
        // Get the model name

        std::vector<std::string> tokens;
        // Split the string by '/'
        size_t start = 0, end = 0;
        while ((end = modelPath_in.find("/", start)) != std::string::npos) {
            tokens.push_back(modelPath_in.substr(start, end - start));
            start = end + 1;
        }
        tokens.push_back(modelPath_in.substr(start));
        // Get the last token
        modelName = tokens.back();
        
        // Create the default shader unless passed one
        if (shader_in == nullptr)
        {
            LOG(WARN, "Object shader null -- Setting default shader");
            // Create heap object shader
            shader = new Shader(paths::object_defaultVertShaderPath, paths::object_defaultFragShaderPath);
        }
        else
        {
            shader = shader_in;
        }
        model = new Model(shader, modelPath_in, base_boundingBox);
    }

    ~ModelObject()
    {
        delete(model);
    }

    void Draw(glm::mat4 view, glm::mat4 projection) override
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

    // For builder, these are object specific as we want to return
    // the object type and also it means we can choose for different
    // objects what the user can see.
    ModelObject* SetPosition(glm::vec3 position_in)
    {
        position = position_in;
        return this;
    }

    ModelObject* SetRotation(glm::vec3 rotation_in)
    {
        rotation = rotation_in;
        return this;
    }

    ModelObject* SetScale(glm::vec3 scale_in)
    {
        scale = scale_in;
        return this;
    }

    ModelObject* SetScale(float scale_in)
    {
        scaleScalar = scale_in;
        return this;
    }

    ModelObject* IsVisible(bool toggle)
    {
        isVisible = toggle;
        return this;
    }

    ModelObject* ShowBoundingBox(bool toggle)
    {
        showBoundingBox = toggle;
        return this;
    }

    ModelObject* SetSpawnOffset(glm::vec3 vec3)
    {
        objectOriginPosition = vec3;
        return this;
    }

    ModelObject* SetModelOriginCenterBottom()
    {
        glm::vec3 center = base_boundingBox->getCenter();
        center.y = base_boundingBox->getMin().y;
        objectOriginPosition = center;
        return this;
    }

    ModelObject* SetModelOriginCenter()
    {
        objectOriginPosition = base_boundingBox->getCenter();
        return this;
    }

    

};
