#pragma once

#include "base_object.hpp"

class LightObject : public BaseObject
{
private:
    glm::vec4 lightColour;
    float intensity;


public:
    LightObject(Shader *shader_in,
                glm::vec3 pos_in = glm::vec3{0.0f, 0.0f, 0.0f},
                glm::vec3 rot_in = glm::vec3{0.0f, 0.0f, 0.0f},
                
                // might need default values for this
                glm::vec4 lightColour_in,
                float intensity_in,

                float isVisible_in = true) :

                BaseObject(shader_in, pos_in, rot_in, isVisible_in),
                lightColour{lightColour_in},
                intensity{intensity_in}
    {
        // constuctor for light renderer
        // this might not be needed due to how easy light is
        // except for the shader
    }


    void Draw(glm::mat4 view, glm::mat4 projection) override
    {

    }

};