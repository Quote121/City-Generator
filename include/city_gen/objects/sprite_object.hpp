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
    
    bool isBillboard = false;                   // Billboard sprite always looking at player
    glm::vec2 scale = {1.0f, 1.0f};             // only scaled in X-axis and Y-axis 
    float scaleScalar = 1.0f;                   // for proportional scaling in x and y
    Sprite* sprite;

    std::string spriteName;

public:
    SpriteObject(std::string& spriteTexture_in,
                 Shader *shader_in);
    ~SpriteObject();

    void Draw(glm::mat4 view, glm::mat4 projection) override;

    // Builders
    SpriteObject* SetPosition(glm::vec3 position_in);
    SpriteObject* SetRotation(glm::vec3 rotation_in);
    SpriteObject* SetScale(glm::vec2 scale_in);
    SpriteObject* SetScale(float scale_in);
    SpriteObject* SetIsVisible(bool toggle);
    SpriteObject* SetIsBillboard(bool toggle);
    SpriteObject* ShowBoundingBox(bool toggle);
    SpriteObject* SetSpawnOffset(glm::vec3 vec3);
    SpriteObject* SetModelOriginCenterBottom();
    SpriteObject* SetModelOriginCenter();
    std::string const& GetSpriteName();
    // ImGui Definitions
    bool& GetIsBillboardImGui();
    glm::vec2& GetScaleImGui();
    float& GetScaleScalarImGui();
};
