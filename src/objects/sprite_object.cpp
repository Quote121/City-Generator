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
    // Get spriteName
    spriteName = spriteTexture_in.substr(spriteTexture_in.find_last_of('/')+1, spriteTexture_in.size()-1);

    this->spriteRenderer = new SpriteRenderer(shader_in, spriteTexture_in);
}


SpriteObject::~SpriteObject()
{
    delete(spriteRenderer);
}


void SpriteObject::Draw(glm::mat4 view, glm::mat4 projection)
{
    // Only draw if the object is visible
    if (isVisible)
    {
        glm::mat4 result = this->GetModelMatrix();
        Shader* objectShader = spriteRenderer->GetSpriteShader();

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

            spriteRenderer->SpriteRenderer::Draw();
        }
    }
}

void SpriteObject::DrawBoundingBox(glm::vec3 colour)
{
    glm::mat4 view = Camera::getInstance()->GetViewMatrix();
    glm::mat4 projection = Camera::getInstance()->GetProjectionMatrix();
    BoundingBox* bb = this->spriteRenderer->GetBoundingBox();
    Shader* bbShader = bb->getShader();
    bbShader->use();
    bbShader->setMat4("view", view);
    bbShader->setMat4("projection", projection);
    bbShader->setMat4("model", this->GetModelMatrix());
    bbShader->setVec3("colour", colour);

    bb->Draw();
}


glm::mat4 SpriteObject::GetModelMatrix(void)
{
    if (isBillboard)
    {
        Camera* cam = Camera::getInstance();
        
        glm::vec3 objectToCamera = -glm::normalize(cam->Position - position);

        glm::vec3 right = glm::normalize(glm::cross(objectToCamera, cam->Up));
        glm::vec3 up = glm::normalize(glm::cross(right, objectToCamera));
        glm::mat4 rotation_new = glm::mat4(glm::vec4(right, 0.0f), glm::vec4(up, 0.0f), glm::vec4(-objectToCamera, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

        // Ignore rotation matrix and apply the other matrix to look at player
        return glm::mat4(1.0f) * getPositionMat4(position) * rotation_new * BaseObject::getScaleMat4(scaleScalar) * BaseObject::getScaleMat4(scale);
    }
    else
    {
        return glm::mat4(1.0f) * getPositionMat4(position) * getRotateMat4(rotation) * BaseObject::getScaleMat4(scaleScalar) * BaseObject::getScaleMat4(scale);
    }
}
const SpriteRenderer* SpriteObject::GetSpriteRenderer(void) const
{
    return spriteRenderer;
}

const BoundingBox* SpriteObject::GetBoundingBox(void) const
{
    return spriteRenderer->GetBoundingBox();
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
    glm::vec3 center = spriteRenderer->GetBoundingBox()->getCenter();
    center.y = spriteRenderer->GetBoundingBox()->getMin().y;
    objectOriginPosition = center;
    return this;
}

SpriteObject* SpriteObject::SetModelOriginCenter()
{
    objectOriginPosition = spriteRenderer->GetBoundingBox()->getCenter();
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
