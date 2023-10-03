#pragma once

#include <glm/glm.hpp> // opengl mathematics
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"
#include "model.hpp"


// The base class of an object for other objects to be created from
class BaseObject {

private:

// Only accessable by classes that inherit
protected:

    glm::vec3 rotation = {0.0f, 0.0f, 0.0f};    // current rotation of the object
    glm::vec3 scale = {1.0f, 1.0f, 1.0f};       // scaled object
    glm::vec3 position = {0.0f, 0.0f, 0.0f};    // Position in 3d space
    float scaleScalar = 1.0f;                   // Float to scale object in all directions
    bool isVisible = true;                      // Visability Flag

    // // Constructor with shader
    // BaseObject(const std::string &objectPath, Shader &shader, glm::vec3 pos_in = glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3 scale_in = glm::vec3{1.0f, 1.0f, 1.0f}, glm::vec3 colour_in = glm::vec3{0.0f, 0.0f, 0.0f})
    // : _objectPath{objectPath}, _position(pos_in), _scale(scale_in), _colour{colour_in, 1.0f} 
    // {
    //     this->setShader(shader);
    //     _model = new Model(objectPath);
    // };

    // ~BaseObject()
    // {
    //     delete(_model);
    //     _shader = nullptr;
    //     _model = nullptr;
    // }



    virtual void Draw();

    // void Draw(Shader &shader)
    // {
    //     _model->Draw(shader);
    // }

    // Set shader already no need to change it
    // Can just pass in view and projection matrices
    void Draw(glm::mat4 view, glm::mat4 projection, glm::vec4 colour = glm::vec4{0.0f, 0.0f, 0.0f, 1.0f});



    /* get mat4 from 3 angles */
    glm::mat4 getRotateMat4(glm::vec3 angles);
    glm::mat4 getPositionMat4(glm::vec3 position);
    glm::mat4 getScaleMat4(glm::vec3 scale);
    glm::mat4 getScaleMat4(float scale);
};
