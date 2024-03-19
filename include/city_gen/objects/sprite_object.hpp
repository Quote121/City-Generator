#pragma once

#include <base_object.hpp>

// Forward declarations
class Sprite;
class Shader;

// 2d sprites like billboards
class SpriteObject : public BaseObject<SpriteObject>
{
private:
    
    bool isBillboard = false;                   // Billboard sprite always looking at player
    glm::vec2 scale = {1.0f, 1.0f};             // only scaled in X-axis and Y-axis 
    Sprite* sprite;

    std::string spriteName;

public:
    // Shader_in can be nullptr
    SpriteObject(const std::string& spriteTexture_in,
                 Shader *shader_in);
    ~SpriteObject();

    void Draw(glm::mat4 view, glm::mat4 projection) override;

    SpriteObject* SetVec2Scale(glm::vec2 scale_in);
    SpriteObject* SetIsBillboard(bool toggle);
    SpriteObject* SetSpawnOffset(glm::vec3 vec3);
    SpriteObject* SetModelOriginCenterBottom();
    SpriteObject* SetModelOriginCenter();
    std::string const& GetSpriteName();
    // ImGui Definitions
    bool& GetIsBillboardImGui();
    glm::vec2& GetScaleImGui();
    float& GetScaleScalarImGui();
};
