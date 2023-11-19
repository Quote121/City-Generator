#pragma once

#include <base_object.hpp>

class DirectionalLightObject : public BaseObject<DirectionalLightObject>
{
private:
    glm::vec3 direction;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    glm::vec3 lightColour = {1.0f, 1.0f, 1.0f};

public:

    DirectionalLightObject()
    {

    }

};