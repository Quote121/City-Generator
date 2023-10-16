#pragma once
#include "base_object.hpp"
#include "line.hpp"

// Line object
class LineObject : public BaseObject
{
private:
    glm::vec3 scale; // Acts as a scale
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
               glm::vec3 colour_in,
               bool isVisible_in,
               glm::vec3 scale_in
               ) :
               BaseObject(glm::vec3{0,0,0}, glm::vec3{0,0,0}, isVisible_in),
               a{point_a},
               b{point_b},
               colour{colour_in},
               scale{scale_in}
    {
        // Determine if we are using the default shader and also the bounding box
        base_boundingBox->Update(point_a);
        base_boundingBox->Update(point_b);
        // position = base_boundingBox->getCenter();

        Shader* shader;

        if (shader_in == nullptr)
        {
            LOG(WARN, "Line shader null -- Using default shader");
            shader = new Shader(paths::line_defaultVertShaderPath, paths::line_defaultFragShaderPath);
        }
        else
        {
            shader = shader_in;
        }
        line_obj = new Line(shader);
        line_obj->UpdateVerts(point_a, point_b);
    }

    ~LineObject()
    {
        delete(line_obj);
    }

    void Draw(glm::mat4 view, glm::mat4 projection) override
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

            line_obj->Line::Draw();
        }

    }
};