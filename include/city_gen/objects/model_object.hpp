#pragma once

#include "base_object.hpp"

#include "config.hpp"

// 3D object
class ModelObject : public BaseObject
{
private:
    glm::vec3 scale = {1.0f, 1.0f, 1.0f};
    Model* model;

    // bounding box?

public:
    // Derived constructor
    ModelObject(std::string& modelPath_in,   // Path to .obj
           Shader *shader_in,                                   
           bool isVisible_in,
           glm::vec3 pos_in,      // No defaults as the scene should deal with that, this is just the model nothing else
           glm::vec3 rot_in,      // 
           glm::vec3 scl_in) :                        

           BaseObject(pos_in, rot_in, isVisible_in), // Base constructor
           scale(scl_in)                                        // initalizer list
    {
        Shader* shader;

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
        glm::mat4 result = glm::mat4(1.0f) * getRotateMat4(rotation) * getPositionMat4(position) * getScaleMat4(scaleScalar) * getScaleMat4(scale);
        Shader* objectShader = model->GetModelShader();

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
            objectShader->setVec3("localCenterPos", base_boundingBox->getCenter());
            model->Model::Draw();
        }
    }

};
