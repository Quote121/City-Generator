#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <glm/glm.hpp> // opengl mathematics
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"
#include "model.hpp"


// An encapsulation of an object
class Object {

private:
    std::string _objectPath;
    glm::vec4 _colour;
    Shader* _shader = nullptr;
    Model* _model;

public:

    glm::vec3 _rotation = {0.0f, 0.0f, 0.0f};
    glm::vec3 _scale = {1.0f, 1.0f, 1.0f};
    glm::vec3 _position = {0.0f, 0.0f, 0.0f};
    float _scaleScalar = 1.0f;

    // Constructor with shader
    Object(const std::string &objectPath, Shader &shader, glm::vec3 pos_in = glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3 scale_in = glm::vec3{1.0f, 1.0f, 1.0f}, glm::vec3 colour_in = glm::vec3{0.0f, 0.0f, 0.0f})
    : _objectPath{objectPath}, _position(pos_in), _scale(scale_in), _colour{colour_in, 1.0f} 
    {
        this->setShader(shader);
        _model = new Model(objectPath);
    };

    ~Object()
    {
        delete(_model);
        _shader = nullptr;
        _model = nullptr;
    }

    void Draw(Shader &shader)
    {
        _model->Draw(shader);
    }

    // Set shader already no need to change it
    // Can just pass in view and projection matrices
    void Draw(glm::mat4 view, glm::mat4 projection, glm::vec4 colour = glm::vec4{0.0f, 0.0f, 0.0f, 1.0f});

    void setShader(Shader &shader)
    {
        _shader = &shader;
    }
    
    glm::vec3 getColour(){
        return glm::vec3(_colour);
    }

    // With alpha channel
    glm::vec4 getColourAlpha(){
        return _colour;
    }

    std::string getFilePath(){
        return _objectPath;
    }



    /* get mat4 from 3 angles */
    glm::mat4 getRotateMat4(glm::vec3 angles);
    glm::mat4 getPositionMat4(glm::vec3 position);
    glm::mat4 getScaleMat4(glm::vec3 scale);
    glm::mat4 getScaleMat4(float scale);
};

#endif // OBJECT_HPP