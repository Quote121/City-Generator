#include "object.hpp"

void Object::Draw(glm::mat4 view, glm::mat4 projection, glm::vec4 colour)
{
    glm::mat4 result = glm::mat4(1.0f) * getRotateMat4(_rotation) * getPositionMat4(_position) * getScaleMat4(_scaleScalar) * getScaleMat4(_scale);
    
    if (_shader == nullptr)
    {
        std::cerr << "NO SHADER LOADED TO OBJECT CLASS" << std::endl;
    }
    else
    {
        // Apply all position and scaling before drawing
        _shader->use();
        _shader->setMat4("view", view);
        _shader->setMat4("projection", projection);
        _shader->setMat4("model", result);
        _shader->setVec4("colour", colour);
        _model->Draw(*_shader);
    }
}

/**
 * pass a vec3 and get a mat4 result
*/
glm::mat4 Object::getRotateMat4(glm::vec3 angles)
{
    glm::mat4 rotation = glm::mat4{
        {glm::cos(angles.z) * glm::cos(angles.y), glm::cos(angles.z)*glm::sin(angles.y)*glm::sin(angles.x) - glm::sin(angles.z)*glm::cos(angles.x), glm::cos(angles.z)*glm::sin(angles.y)*glm::cos(angles.x) + glm::sin(angles.z)*glm::sin(angles.x), 0 },
        {glm::sin(angles.z)*glm::cos(angles.y), glm::sin(angles.z)*glm::sin(angles.y)*glm::sin(angles.x) + glm::cos(angles.z)*glm::cos(angles.x), glm::sin(angles.z)*glm::sin(angles.y)*glm::cos(angles.x) - glm::cos(angles.z)*glm::sin(angles.x), 0 },
        {-glm::sin(angles.y), glm::cos(angles.y)*glm::sin(angles.x), glm::cos(angles.y)*glm::cos(angles.x), 0},
        {0, 0, 0, 1}
    };
    return rotation;
}

glm::mat4 Object::getPositionMat4(glm::vec3 position)
{
    glm::mat4 matrix(1.0f);
    matrix[3] = glm::vec4(position, 1.0f);
    return matrix;
}

glm::mat4 Object::getScaleMat4(glm::vec3 scale)
{
    // identity
    glm::mat4 matrix(1.0f);
    matrix[0][0] *= scale.x;
    matrix[1][1] *= scale.y;
    matrix[2][2] *= scale.z;

    return matrix;
}

glm::mat4 Object::getScaleMat4(float scale)
{
    glm::mat4 matrix(1.0f);
    matrix[0][0] *= scale;
    matrix[1][1] *= scale;
    matrix[2][2] *= scale;
    return matrix;
}
