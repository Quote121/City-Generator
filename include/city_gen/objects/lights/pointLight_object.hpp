#pragma once

// #include <base_object.hpp>
#include <sprite_object.hpp>
#include <config.hpp>

// Inherit sprite object so that we can show sprite where light would be
class PointLightObject : public SpriteObject
{
private:
    glm::vec3 lightColour = {1.0f, 1.0f, 1.0f};
    // TODO incorporate this
    float intensity = 1.0f;

    // Make constructor for these values
    // glm::vec3 ambient = {0.05f, 0.05f, 0.05f};
    glm::vec3 ambient = {0.1f, 0.1f, 0.1f};

    glm::vec3 diffuse = {1.0f, 1.0f, 1.0f};
    glm::vec3 specular = {0.07f, 0.07f, 0.07f};
    
    // Seem to be the same
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;

    

public:
    // Pass light texture to where the light is
    PointLightObject() : SpriteObject(paths::light_icon_texture, nullptr)
    {
        // SetModelOriginCenter();
        SetIsBillboard(true);
    }

    // Getters
    glm::vec3 GetAmbient()
    {
        return lightColour * ambient;
    }

    glm::vec3 GetDiffuse()
    {
        return lightColour * diffuse;
    }

    glm::vec3 GetSpecular()
    {
        return lightColour * specular;
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

    // builders for light properties
    PointLightObject* SetLightColour(glm::vec3 colour_in)
    {
        lightColour = colour_in;
        return this;
    }

    PointLightObject* SetAmbient(glm::vec3 ambient_in)
    {
        ambient = ambient_in;
        return this;
    }
    
    PointLightObject* SetDiffuse(glm::vec3 diffuse_in)
    {
        diffuse = diffuse_in;
        return this;
    }

    PointLightObject* SetSpecular(glm::vec3 specular_in)
    {
        specular = specular_in;
        return this;
    }

    PointLightObject* SetConstant(float constant_in)
    {
        constant = constant_in;
        return this;
    }
    
    PointLightObject* SetLinear(float linear_in)
    {
        linear = linear_in;
        return this;
    }

    PointLightObject* SetQuadratic(float quadratic_in)
    {
        quadratic = quadratic_in;
        return this;
    }

    // ImGui handle getters
    glm::vec3& GetLightColourImGui()
    {
        return lightColour;
    }

    glm::vec3& GetAmbientImGui()
    {
        return ambient;
    }

    glm::vec3& GetDiffuseImGui()
    {
        return diffuse;
    }

    glm::vec3& GetSpecularImGui()
    {
        return specular;
    }

    float& GetIntensityImGui()
    {
        return intensity;
    }

    float& GetConstantImGui()
    {
        return constant;
    }

    float& GetLinearImGui()
    {
        return linear;
    }

    float& GetQuadraticImGui()
    {
        return quadratic;
    }

};
