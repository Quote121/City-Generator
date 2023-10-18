#pragma once

#include <glm/glm.hpp> // opengl mathematics
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"
#include "model.hpp"
#include "bounding_box.hpp"
#include "config.hpp"

// The base class of an object for other objects to be created from
class BaseObject {

// Only accessable by classes that inherit
protected:
    glm::vec3 rotation = {0.0f, 0.0f, 0.0f};    // current rotation of the object
    glm::vec3 position = {0.0f, 0.0f, 0.0f};    // Position in 3d space
    
    BoundingBox* base_boundingBox; // Info is local verticies
    bool showBoundingBox = true; // Will draw the outline of the bounding box on the model

    float scaleScalar = 1.0f;                   // Float to scale object in all directions
    bool isVisible = true;                      // Visability Flag


    BaseObject(glm::vec3 pos_in = glm::vec3{0.0f, 0.0f, 0.0f},
               glm::vec3 rot_in = glm::vec3{0.0f, 0.0f, 0.0f},
               bool isVisible_in = true) :
               position{pos_in}, rotation{rot_in}, isVisible{isVisible_in}
    {
        base_boundingBox = new BoundingBox();
    }

    ~BaseObject()
    {
        delete base_boundingBox;
    }

    // No definition just a virtual function for derived objects
    virtual void Draw(glm::mat4 view, glm::mat4 projection) {};

    /* get mat4 from 3 angles */
    glm::mat4 getRotateMat4(glm::vec3 angles);
    glm::mat4 getPositionMat4(glm::vec3 position);
    
    glm::mat4 getScaleMat4(glm::vec3 scale);
    glm::mat4 getScaleMat4(glm::vec2 scale); // For sprites
    glm::mat4 getScaleMat4(float scale);
};
