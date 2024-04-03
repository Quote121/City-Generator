#pragma once

#include <base_object.hpp>
#include <camera.hpp>
#include <bounding_box.hpp>

// Forward declarations
class SpriteRenderer;
class Shader;

// 2d sprites like billboards
class SpriteObject : public BaseObject<SpriteObject>
{
private:
    
    bool isBillboard = false;                   // Billboard sprite always looking at player
    glm::vec2 scale = {1.0f, 1.0f};             // only scaled in X-axis and Y-axis 
    SpriteRenderer* spriteRenderer;
    
    bool isInstanceRenderered = false;
    bool lightingEnable = false;

    std::string spritePath;
    std::string spriteName;

public:
    // Shader_in can be nullptr
    SpriteObject(const std::string& spriteTexture_in,
                 Shader *shader_in);
    ~SpriteObject();

    void Draw(glm::mat4 view, glm::mat4 projection) override;
    void DrawInstances(glm::mat4 view, glm::mat4 projection, std::vector<float>* matrices);
    void DrawBoundingBox(glm::vec3 colour);

    glm::mat4 GetModelMatrix(void);
    const SpriteRenderer* GetSpriteRenderer(void) const;
    const BoundingBox* GetBoundingBox(void) const;

    SpriteObject* SetVec2Scale(glm::vec2 scale_in);
    SpriteObject* SetIsBillboard(bool toggle);
    SpriteObject* SetSpawnOffset(glm::vec3 vec3);
    SpriteObject* SetModelOriginCenterBottom();
    SpriteObject* SetModelOriginCenter();
    SpriteObject* SetLightingEnabled(bool toggle);
    std::string const& GetSpriteName();
    std::string const& GetSpritePath();
    
    glm::vec2 GetScale(void) const
    {
        return scale;
    }

    bool GetIsInstanceRendered(void) const
    {
        return isInstanceRenderered;
    }

    void SetIsInstanceRendered(bool toggle)
    {
        this->isInstanceRenderered = toggle;
    }

    // ImGui Definitions
    bool& GetIsBillboardImGui();
    glm::vec2& GetScaleImGui();
    float& GetScaleScalarImGui();
    bool& GetLightingEnabledImGui();
};
