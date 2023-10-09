#pragma once

#include "base_object.hpp"
#include "object.hpp"

#include "config.hpp"

// 3D object
class Object : public BaseObject
{
private:
    glm::vec3 scale = {1.0f, 1.0f, 1.0f};
    Model* model;

    // bounding box?

public:
    // Derived constructor
    Object(std::string& modelPath_in,                           // Path to obj file for model
           Shader *shader_in,                                   // Shader to associate with the draw of this object
           float isVisible_in = true,
           glm::vec3 pos_in = glm::vec3{0.0f, 0.0f, 0.0f},      // Starting pos of the object, defualt origin
           glm::vec3 rot_in = glm::vec3{0.0f, 0.0f, 0.0f},      // 
           glm::vec3 scl_in = glm::vec3{1.0f, 1.0f, 1.0f}) :                         // if the object is visible
           
           BaseObject(pos_in, rot_in, isVisible_in), // Base constructor
           scale(scl_in)                                        // initalizer list
    {
        Shader* shader;

        // Create the default shader unless passed one
        if (shader_in == nullptr)
        {
            // Create heap object shader
            shader = new Shader(object_defaultVertShaderPath, object_defaultFragShaderPath);
        }
        else
        {
            shader = shader_in;
        }
        model = new Model(shader, modelPath_in);
    }

    ~Object()
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
            model->Model::Draw();
        }
    }

};