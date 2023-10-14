#include "base_object.hpp"

// TODO check it works
glm::mat4 BaseObject::getScaleMat4(glm::vec2 scale)
{
    // identity
    glm::mat4 matrix(1.0f);
    matrix[0][0] *= scale.x;
    matrix[1][1] *= scale.y;

    return matrix;
}

/**
 * pass a vec3 and get a mat4 result
*/
glm::mat4 BaseObject::getRotateMat4(glm::vec3 angles)
{
    glm::mat4 rotation = glm::mat4{
        {glm::cos(angles.z) * glm::cos(angles.y), glm::cos(angles.z)*glm::sin(angles.y)*glm::sin(angles.x) - glm::sin(angles.z)*glm::cos(angles.x), glm::cos(angles.z)*glm::sin(angles.y)*glm::cos(angles.x) + glm::sin(angles.z)*glm::sin(angles.x), 0 },
        {glm::sin(angles.z)*glm::cos(angles.y), glm::sin(angles.z)*glm::sin(angles.y)*glm::sin(angles.x) + glm::cos(angles.z)*glm::cos(angles.x), glm::sin(angles.z)*glm::sin(angles.y)*glm::cos(angles.x) - glm::cos(angles.z)*glm::sin(angles.x), 0 },
        {-glm::sin(angles.y), glm::cos(angles.y)*glm::sin(angles.x), glm::cos(angles.y)*glm::cos(angles.x), 0},
        {0, 0, 0, 1}
    };
    return rotation;
}

glm::mat4 BaseObject::getPositionMat4(glm::vec3 position)
{
    glm::mat4 matrix(1.0f);
    return glm::translate(matrix, position);
}

glm::mat4 BaseObject::getScaleMat4(glm::vec3 scale)
{
    // identity
    glm::mat4 matrix(1.0f);
    matrix[0][0] *= scale.x;
    matrix[1][1] *= scale.y;
    matrix[2][2] *= scale.z;

    return matrix;
}

glm::mat4 BaseObject::getScaleMat4(float scale)
{
    glm::mat4 matrix(1.0f);
    matrix[0][0] *= scale;
    matrix[1][1] *= scale;
    matrix[2][2] *= scale;
    return matrix;
}
