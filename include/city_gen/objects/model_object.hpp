#pragma once

#include <base_object.hpp>

// Forward declarations
class Model;
class Shader;

// 3D object
class ModelObject : public BaseObject<ModelObject>
{
private:
    glm::vec3 scale = {1.0f, 1.0f, 1.0f};
    Model* model;

    // Will draw the outline of the bounding box on the model
    bool showBoundingBox = true;

    bool lightingEnable = false;

public:
    // modelPath_in -- Path to the model's .obj
    // shader_in -- path to models vertex shader
    ModelObject(const std::string& modelPath_in,   // Path to .obj
                Shader* shader_in);                       

    ~ModelObject();

    void Draw(glm::mat4 view, glm::mat4 projection) override;

    // For builder, these are object specific as we want to return
    // the object type and also it means we can choose for different
    // objects what the user can see.
    ModelObject* SetPosition(glm::vec3 position_in);
    ModelObject* SetRotation(glm::vec3 rotation_in);
    ModelObject* SetScale(glm::vec3 scale_in);
    ModelObject* SetScale(float scale_in);
    ModelObject* IsVisible(bool toggle);
    ModelObject* SetLightingEnabled(bool toggle);

    ModelObject* ShowBoundingBox(bool toggle);
    ModelObject* SetSpawnOffset(glm::vec3 vec3);
    ModelObject* SetModelOriginCenterBottom();
    ModelObject* SetModelOriginCenter();

    // Getters
    std::string GetModelName() const;
    bool GetShowBoundingBox() const;
    
    // ImGui
    glm::vec3& GetScaleImGui();
    bool& GetShowBoundingBoxImGui();
    bool& GetShowLightingImGui();
};
