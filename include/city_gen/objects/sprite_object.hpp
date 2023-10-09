#pragma once

#include "base_object.hpp"
#include "sprite.hpp"
#include "config.hpp" // For shader paths

/*
    The object classes of these are more about the physicl properties
    of the object. For things like shaders and texture information that
    will be stored in the Sprite class below in the renderer

    This goes for all objects
*/



// 2d sprites like bilboards
class SpriteObject : public BaseObject
{
private:
    
    bool isBillboard = false;                   // Billboard sprite always looking at player
    glm::vec2 scale = {1.0f, 1.0f};             // only scaled in X-axis and Y-axis 
    float scaleScalar = 1.0f; // for proportional scaling in x and y
    Sprite* sprite;
    // bounding box
    // The bounding box of this is the dimentions of the texture
    // so its quite easy to implement

    //
    // We take in a texture
    // We determine the ratio of its x and y.
    // We want to accept formats with transparancy
    //
    // We then take that ratio to plot a quad to then display that texture
    // 
    // The constructor of sprite.hpp will determine the above
    // The Draw call below will call into sprite.hpp where the openGL code will
    // draw our sprite
    //

public:
    // only parameter is spriteTexture_in
    // *shader_in can be NULL
    SpriteObject(std::string& spriteTexture_in,
                 Shader *shader_in,
                 float isVisible_in = true,
                 bool isBillboard_in = false,
                 glm::vec3 pos_in = glm::vec3{0.0f, 0.0f, 0.0f},      // Starting pos of the object, defualt origin
                 glm::vec3 rot_in = glm::vec3{0.0f, 0.0f, 0.0f},      // 
                 glm::vec2 scl_in = glm::vec2{1.0f, 1.0f}) : 
                 
                 BaseObject(pos_in, rot_in, isVisible_in),
                 isBillboard{isBillboard_in}
    {
        Shader* shader;

        // Create the default shader unless passed one
        if (shader_in == nullptr)
        {
            // Create heap object shader
            shader = new Shader(sprite_defaultVertShaderPath, sprite_defaultFragShaderPath);
        }
        else
        {
            shader = shader_in;
        }

        sprite = new Sprite(shader, spriteTexture_in);
    }

    ~SpriteObject()
    {
        delete(sprite);
    }

    void Draw(glm::mat4 view, glm::mat4 projection) override
    {
        glm::mat4 result;
        Shader* objectShader = sprite->GetSpriteShader();

        // if billbord texture
        if (isBillboard)
        {
            // Ignore rotation matrix and apply the other matrix to look at player
            // TODO change
            result = glm::mat4(1.0f) * getRotateMat4(rotation) * getPositionMat4(position) * BaseObject::getScaleMat4(scaleScalar) * BaseObject::getScaleMat4(scale);
        }
        else
        {
            result = glm::mat4(1.0f) * getRotateMat4(rotation) * getPositionMat4(position) * BaseObject::getScaleMat4(scaleScalar) * BaseObject::getScaleMat4(scale);
        }
    
        

        if (objectShader == nullptr)
        {
            std::cerr << "NO SHADER LOADED TO OBJECT CLASS" << std::endl;
        }
        else
        {
            // Apply all position and scaling before drawing
            
            // Shader stuff should be moved to the render class
            objectShader->use();
            objectShader->setMat4("view", view);
            objectShader->setMat4("projection", projection);
            objectShader->setMat4("model", result);

            sprite->Sprite::Draw();
        }
    }

};
