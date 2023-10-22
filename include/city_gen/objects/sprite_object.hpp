#pragma once

#include "base_object.hpp"
#include "sprite.hpp"
#include "config.hpp" // For shader paths
#include "camera.hpp"
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
    
    // TODO add alias based on texture name e.g. wall_01 or grass_03

    bool isBillboard = false;                   // Billboard sprite always looking at player
    glm::vec2 scale = {1.0f, 1.0f};             // only scaled in X-axis and Y-axis 
    float scaleScalar = 1.0f;                   // for proportional scaling in x and y
    Sprite* sprite;

public:
    SpriteObject(std::string& spriteTexture_in,
                Shader *shader_in) : 
                BaseObject()
    {
        Shader* shader;

        // Create the default shader unless passed one
        if (shader_in == nullptr)
        {
            LOG(WARN, "Sprite shader passed null -- using default shader")
            // Create heap object shader
            shader = new Shader(paths::sprite_defaultVertShaderPath, paths::sprite_defaultFragShaderPath);
        }
        else
        {
            shader = shader_in;
        }

        base_boundingBox = new BoundingBox();
        sprite = new Sprite(shader, spriteTexture_in, base_boundingBox);
    }

    ~SpriteObject()
    {
        delete(sprite);
        delete(base_boundingBox);
    }

    void Draw(glm::mat4 view, glm::mat4 projection) override
    {
        // Only draw if the object is visible
        if (isVisible)
        {
        
        glm::mat4 result;
        Shader* objectShader = sprite->GetSpriteShader();

        // if billbord texture
        if (isBillboard)
        {
            Camera* cam = Camera::getInstance();
            
            glm::vec3 objectToCamera = -glm::normalize(cam->Position - position);
            glm::vec3 right = glm::normalize(glm::cross(objectToCamera, cam->Up));
            glm::vec3 up = glm::normalize(glm::cross(right, objectToCamera));
            glm::mat4 rotation_new = glm::mat4(glm::vec4(right, 0.0f), glm::vec4(up, 0.0f), glm::vec4(-objectToCamera, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

            // Ignore rotation matrix and apply the other matrix to look at player
            result = glm::mat4(1.0f) * getPositionMat4(position) * rotation_new * BaseObject::getScaleMat4(scaleScalar) * BaseObject::getScaleMat4(scale);
        }
        else
        {
            result = glm::mat4(1.0f) * getPositionMat4(position) * getRotateMat4(rotation) * BaseObject::getScaleMat4(scaleScalar) * BaseObject::getScaleMat4(scale);
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
            objectShader->setVec3("localCenterPos", objectOriginPosition);

            sprite->Sprite::Draw();
        }
        }
    }

    // Builders
    SpriteObject* SetPosition(glm::vec3 position_in)
    {
        position = position_in;
        return this;
    }

    SpriteObject* SetRotation(glm::vec3 rotation_in)
    {
        rotation = rotation_in;
        return this;
    }

    SpriteObject* SetScale(glm::vec2 scale_in)
    {
        scale = scale_in;
        return this;
    }

    SpriteObject* SetScale(float scale_in)
    {
        scaleScalar = scale_in;
        return this;
    }

    SpriteObject* SetIsVisible(bool toggle)
    {
        isVisible = toggle;
        return this;
    }

    SpriteObject* SetIsBillboard(bool toggle)
    {
        isBillboard = toggle;
        return this;
    }

    SpriteObject* ShowBoundingBox(bool toggle)
    {
        showBoundingBox = toggle;
        return this;
    }

    SpriteObject* SetSpawnOffset(glm::vec3 vec3)
    {
        objectOriginPosition = vec3;
        return this;
    }

    SpriteObject* SetModelOriginCenterBottom()
    {
        glm::vec3 center = base_boundingBox->getCenter();
        center.y = base_boundingBox->getMin().y;
        objectOriginPosition = center;
        return this;
    }

    SpriteObject* SetModelOriginCenter()
    {
        objectOriginPosition = base_boundingBox->getCenter();
        return this;
    }

};
