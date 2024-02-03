#pragma once

#include <string>
#include <glm/glm.hpp>

// Position is not needed so we are not inheriting from base class with this light
class DirectionalLightObject
{
private:
    std::string alias;

    glm::vec3 direction = {0.0f, 0.0f, 0.0f};

    glm::vec3 ambient = {0.5f, 0.5f, 0.5f};
    glm::vec3 diffuse = {0.5f, 0.5f, 0.5f};
    glm::vec3 specular = {0.5f, 0.5f, 0.5f};

    glm::vec3 lightColour = {1.0f, 1.0f, 1.0f};

public:

    DirectionalLightObject()
    {

    }

    void SetAlias(const std::string* alias_in)
    {
        alias = *alias_in;
    }
    inline std::string& GetAlias()
    {
        return alias;
    }

    // Getters
    glm::vec3 GetDirection()
    {
        return direction;
    }

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

    // ImGui handle getters
    glm::vec3& GetLightColourImGui()
    {
        return lightColour;
    }

    glm::vec3& GetDirectionImGui()
    {
        return direction;
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

    // Builders

    DirectionalLightObject* SetDirection(glm::vec3 direction_in)
    {
        direction = direction_in;
        return this;
    }


    DirectionalLightObject* SetColour(glm::vec3 colour_in)
    {
        lightColour = colour_in;
        return this;
    }


    DirectionalLightObject* SetAmbient(glm::vec3 ambient_in)
    {
        ambient = ambient_in;
        return this;
    }
    

    DirectionalLightObject* SetDiffuse(glm::vec3 diffuse_in)
    {
        diffuse = diffuse_in;
        return this;
    }


    DirectionalLightObject* SetSpecular(glm::vec3 specular_in)
    {
        specular = specular_in;
        return this;
    }
};
