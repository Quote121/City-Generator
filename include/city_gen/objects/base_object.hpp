#pragma once

#include <glm/glm.hpp> // opengl mathematics
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <config.hpp>

// Forward declaration
class BoundingBox;

// The base class of an object for other objects to be created from
template <class T>
class BaseObject {

// Only accessable by classes that inherit
protected:
    // Alias given by the scene handler
    // e.g. line_7, sprite_12
    // 3D assets will also have a filepath for the asset its loading
    std::string alias;

    glm::vec3 rotation = {0.0f, 0.0f, 0.0f};    // current rotation of the object
    glm::vec3 position = {0.0f, 0.0f, 0.0f};    // Position in 3d space
    
    glm::vec3 objectOriginPosition = {0.0f, 0.0f, 0.0f}; // Position offset for spawning. E.g. when spawning at 0,0,0 the offset can be set
                                                         // to the center of the object or the center of the bottom so its easier to place
    
    float scaleScalar = 1.0f;                   // Float to scale object in all directions
    bool isVisible = true;                      // Visability Flag

    BaseObject();
    virtual ~BaseObject();

    // No definition just a virtual function for derived objects
    virtual void Draw(glm::mat4 view, glm::mat4 projection) {};

    /* get mat4 from 3 angles */
    glm::mat4 getRotateMat4(glm::vec3 angles) const;
    glm::mat4 getPositionMat4(glm::vec3 position) const;
    
    glm::mat4 getScaleMat4(glm::vec3 scale) const;
    glm::mat4 getScaleMat4(glm::vec2 scale) const; // For sprites
    glm::mat4 getScaleMat4(float scale) const;
public:
    float GetDistanceFromCamera() const;

    void SetAlias(const std::string* alias_in)
    {
        alias = *alias_in;
    }
    inline std::string& GetAlias()
    {
        return alias;
    }

    glm::vec3 GetPosition() const
    {
        return position;
    }

    glm::vec3 GetRotation() const
    {
        return rotation;
    }

    bool GetIsVisible() const
    {
        return isVisible;
    }

    float GetScaleScalar() const
    {
        return scaleScalar;
    }



    // ImGui getters. These return a reference to each data type
    // as the imgui interface requires it for callbacks.
    // since this is a single threaded application we dont have
    // to worry about race cases etc.

    glm::vec3& GetPositionImGui()
    {
        return position;
    }

    glm::vec3& GetRotationImGui()
    {
        return rotation;
    }

    bool& GetIsVisibleImGui()
    {
        return isVisible;
    }


    float& GetScaleScalarImGui()
    {
        return scaleScalar;
    }


    // Builders
    T* SetPosition(glm::vec3 position_in);
    T* SetRotation(glm::vec3 rotation_in);
    // T* SetScale(glm::vec2 scale_in);
    T* SetScale(float scale_in);
    T* SetIsVisible(bool toggle);
    // T* ShowBoundingBox(bool toggle);
};

// template class BaseObject<SpriteObject>;

// #include "../src/objects/base_object.cpp"
