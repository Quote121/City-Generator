#include <sprite_object.hpp>

#include <shader.hpp>
#include <resourceManager.hpp>
#include <sprite.hpp>
#include <camera.hpp>
#include <bounding_box.hpp>

SpriteObject::SpriteObject(const std::string& spriteTexture_in,
            Shader *shader_in) : 
            BaseObject()
{
    Shader* shader;
    ResourceManager* RM = ResourceManager::getInstance();

    // Get spriteName
    spriteName = spriteTexture_in.substr(spriteTexture_in.find_last_of('/')+1, spriteTexture_in.size()-1);

    // Create the default shader unless passed one
    if (shader_in == nullptr)
    {
        // Create heap object shader
        shader = RM->LoadShader(paths::sprite_defaultVertShaderPath, paths::sprite_defaultFragShaderPath);
    }
    else
    {
        // Already created shader that has been passed in
        shader = shader_in;
    }

    sprite = new Sprite(shader, spriteTexture_in);
}


SpriteObject::~SpriteObject()
{
    delete(sprite);
}


void SpriteObject::Draw(glm::mat4 view, glm::mat4 projection)
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


SpriteObject* SpriteObject::SetVec2Scale(glm::vec2 scale_in)
{
    scale = scale_in;
    return this;
}

SpriteObject* SpriteObject::SetIsBillboard(bool toggle)
{
    isBillboard = toggle;
    return this;
}

SpriteObject* SpriteObject::SetSpawnOffset(glm::vec3 vec3)
{
    objectOriginPosition = vec3;
    return this;
}

SpriteObject* SpriteObject::SetModelOriginCenterBottom()
{
    glm::vec3 center = sprite->GetBoundingBox()->getCenter();
    center.y = sprite->GetBoundingBox()->getMin().y;
    objectOriginPosition = center;
    return this;
}

SpriteObject* SpriteObject::SetModelOriginCenter()
{
    objectOriginPosition = sprite->GetBoundingBox()->getCenter();
    return this;
}

std::string const& SpriteObject::GetSpriteName()
{
    return spriteName;
}

// ImGui Definitions

bool& SpriteObject::GetIsBillboardImGui()
{
    return isBillboard;
}

glm::vec2& SpriteObject::GetScaleImGui()
{
    return scale;
}

float& SpriteObject::GetScaleScalarImGui()
{
    return scaleScalar;
}