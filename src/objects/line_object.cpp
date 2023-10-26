#include "line_object.hpp"


LineObject::LineObject(Shader* shader_in,
            glm::vec3 point_a,
            glm::vec3 point_b,
            glm::vec3 colour_in) :
            BaseObject(),
            a{point_a},
            b{point_b},
            colour{colour_in}
{
    // Determine if we are using the default shader and also the bounding box
    base_boundingBox->Update(point_a);
    base_boundingBox->Update(point_b);

    Shader* shader;

    if (shader_in == nullptr)
    {
        LOG(WARN, "Line shader null -- Using default shader");
        shader = ResourceManager::getInstance()->LoadShader(paths::line_defaultVertShaderPath, paths::line_defaultFragShaderPath);
    }
    else
    {
        shader = shader_in;
    }
    line_obj = new Line(shader);
    line_obj->UpdateVerts(point_a, point_b);
}

LineObject::~LineObject()
{
    delete(line_obj);
}

void LineObject::Draw(glm::mat4 view, glm::mat4 projection)
{
    if (isVisible)
    {
        glm::mat4 result = glm::mat4(1.0f) * getRotateMat4(rotation) * getScaleMat4(scaleScalar) * getScaleMat4(scale);
        Shader* objectShader = line_obj->GetLineShader();

        if (objectShader == nullptr)
        {
            std::cerr << "NO SHADER LOADED TO OBJECT CLASS" << std::endl;
        }
        else
        {
            // Apply all position and scaling before drawing
            objectShader->use();
            objectShader->setMat4("view", view);
            objectShader->setMat4("projection", projection);
            objectShader->setMat4("model", result);

            objectShader->setVec3("colour", colour);
            line_obj->UpdateVerts(a, b);
            line_obj->Line::Draw();
        }
    }
}

LineObject* LineObject::SetColour(glm::vec3 colour_in)
{
    colour = colour_in;
    return this;
}

LineObject* LineObject::SetPosition(glm::vec3 position_in)
{
    position = position_in;
    return this;
}

LineObject* LineObject::SetRotation(glm::vec3 rotation_in)
{
    rotation = rotation_in;
    return this;
}

LineObject* LineObject::SetScale(glm::vec3 scale_in)
{
    scale = scale_in;
    return this;
}

LineObject* LineObject::SetScale(float scale_in)
{
    scaleScalar = scale_in;
    return this;
}

LineObject* LineObject::IsVisible(bool toggle)
{
    isVisible = toggle;
    return this;
}

LineObject* LineObject::ShowBoundingBox(bool toggle)
{
    showBoundingBox = toggle;
    return this;
}

LineObject* LineObject::SetSpawnOffset(glm::vec3 vec3)
{
    objectOriginPosition = vec3;
    return this;
}

LineObject* LineObject::SetModelOriginCenterBottom()
{
    glm::vec3 center = base_boundingBox->getCenter();
    center.y = base_boundingBox->getMin().y;
    objectOriginPosition = center;
    return this;
}

LineObject* LineObject::SetModelOriginCenter()
{
    objectOriginPosition = base_boundingBox->getCenter();
    return this;
}

// ImGui

glm::vec3& LineObject::GetScaleImGui()
{
    return scale;
}

glm::vec3& LineObject::GetPointAImGui()
{
    return a;
}

glm::vec3& LineObject::GetPointBImGui()
{
    return b;
}

glm::vec3& LineObject::GetColourImGui()
{
    return colour;
}
