#pragma once

#include <base_object.hpp>
#include <line.hpp>

// Line object
class LineObject : public BaseObject
{
private:
    glm::vec3 scale = {1.0f, 1.0f, 1.0f}; // Acts as a scale
    glm::vec3 colour;

    glm::vec3 a; // The two vertices that describe the line
    glm::vec3 b;

    Line* line_obj;

    // position vec3 from the baseobject isnt set by the user
    // The two line points are
    //
    // The position is determined from the bounding box

public:
    LineObject(Shader* shader_in,
               glm::vec3 point_a,
               glm::vec3 point_b,
               glm::vec3 colour_in);

    ~LineObject();

    void Draw(glm::mat4 view, glm::mat4 projection) override;

    // Builders
    LineObject* SetColour(glm::vec3 colour_in);
    LineObject* SetPosition(glm::vec3 position_in);
    LineObject* SetRotation(glm::vec3 rotation_in);
    LineObject* SetScale(glm::vec3 scale_in);
    LineObject* SetScale(float scale_in);
    LineObject* IsVisible(bool toggle);
    LineObject* ShowBoundingBox(bool toggle);
    LineObject* SetSpawnOffset(glm::vec3 vec3);
    LineObject* SetModelOriginCenterBottom();
    LineObject* SetModelOriginCenter();

    // ImGui
    glm::vec3& GetScaleImGui();
    glm::vec3& GetPointAImGui();
    glm::vec3& GetPointBImGui();
    glm::vec3& GetColourImGui();

};
