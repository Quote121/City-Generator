#pragma once

#include <base_object.hpp>
#include <sprite_object.hpp>

// Inherit sprite object so that we can show sprite where light would be
class PointLightObject : public BaseObject<PointLightObject>
{
private:
    // TODO implement into shaders
    glm::vec3 lightColour = {1.0f, 1.0f, 1.0f};
    float intensity = 1.0f;

    // Make constructor for these values
    // glm::vec3 ambient = {0.05f, 0.05f, 0.05f};
    glm::vec3 ambient = {0.5f, 0.5f, 0.5f};

    glm::vec3 diffuse = {0.08f, 0.08f, 0.08f};
    glm::vec3 specular = {1.0f, 1.0f, 1.0f};
    
    // Seem to be the same
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;

    

public:
    // Pass light texture to where the light is
    PointLightObject() //: SpriteObject(paths::light_icon_texture, nullptr)
    {
        
    }

    // Getters
    glm::vec3 GetAmbient()
    {
        return ambient;
    }

    glm::vec3 GetDiffuse()
    {
        return diffuse;
    }

    glm::vec3 GetSpecular()
    {
        return specular;
    }

    float GetConstant()
    {
        return constant;
    }

    float GetLinear()
    {
        return linear;
    }

    float GetQuadratic()
    {
        return quadratic;
    }

    // TODO add builders for light properties

};
