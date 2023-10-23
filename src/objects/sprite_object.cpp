#include "sprite_object.hpp"

SpriteObject::SpriteObject(std::string& spriteTexture_in,
            Shader *shader_in) : 
            BaseObject()
{
    Shader* shader;

    std::vector<std::string> tokens;
    // Split the string by '/'
    size_t start = 0, end = 0;
    while ((end = spriteTexture_in.find("/", start)) != std::string::npos) {
        tokens.push_back(spriteTexture_in.substr(start, end - start));
        start = end + 1;
    }
    tokens.push_back(spriteTexture_in.substr(start));
    // Get the last token
    spriteName = tokens.back();

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


SpriteObject::~SpriteObject()
{
    delete(sprite);
    delete(base_boundingBox);
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


// Builders
SpriteObject* SpriteObject::SetPosition(glm::vec3 position_in)
{
    position = position_in;
    return this;
}

SpriteObject* SpriteObject::SetRotation(glm::vec3 rotation_in)
{
    rotation = rotation_in;
    return this;
}

SpriteObject* SpriteObject::SetScale(glm::vec2 scale_in)
{
    scale = scale_in;
    return this;
}

SpriteObject* SpriteObject::SetScale(float scale_in)
{
    scaleScalar = scale_in;
    return this;
}

SpriteObject* SpriteObject::SetIsVisible(bool toggle)
{
    isVisible = toggle;
    return this;
}

SpriteObject* SpriteObject::SetIsBillboard(bool toggle)
{
    isBillboard = toggle;
    return this;
}

SpriteObject* SpriteObject::ShowBoundingBox(bool toggle)
{
    showBoundingBox = toggle;
    return this;
}

SpriteObject* SpriteObject::SetSpawnOffset(glm::vec3 vec3)
{
    objectOriginPosition = vec3;
    return this;
}

SpriteObject* SpriteObject::SetModelOriginCenterBottom()
{
    glm::vec3 center = base_boundingBox->getCenter();
    center.y = base_boundingBox->getMin().y;
    objectOriginPosition = center;
    return this;
}

SpriteObject* SpriteObject::SetModelOriginCenter()
{
    objectOriginPosition = base_boundingBox->getCenter();
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