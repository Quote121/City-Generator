#include <base_object.hpp>

#include <camera.hpp>
#include <all.hpp>

#include <road_object.hpp>

// template class BaseObject<PointLightObject>;
template class BaseObject<SpriteObject>;
template class BaseObject<LineObject>;
template class BaseObject<ModelObject>;
// template class BaseObject<RoadObject>;




template<class T>
BaseObject<T>::BaseObject() {}

template<class T>
BaseObject<T>::~BaseObject() {}

template<class T>
glm::mat4 BaseObject<T>::getScaleMat4(glm::vec2 scale) const
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
template<class T>
glm::mat4 BaseObject<T>::getRotateMat4(glm::vec3 angles) const
{
    glm::mat4 rotation = glm::mat4{
        {glm::cos(angles.z) * glm::cos(angles.y), glm::cos(angles.z)*glm::sin(angles.y)*glm::sin(angles.x) - glm::sin(angles.z)*glm::cos(angles.x), glm::cos(angles.z)*glm::sin(angles.y)*glm::cos(angles.x) + glm::sin(angles.z)*glm::sin(angles.x), 0 },
        {glm::sin(angles.z)*glm::cos(angles.y), glm::sin(angles.z)*glm::sin(angles.y)*glm::sin(angles.x) + glm::cos(angles.z)*glm::cos(angles.x), glm::sin(angles.z)*glm::sin(angles.y)*glm::cos(angles.x) - glm::cos(angles.z)*glm::sin(angles.x), 0 },
        {-glm::sin(angles.y), glm::cos(angles.y)*glm::sin(angles.x), glm::cos(angles.y)*glm::cos(angles.x), 0},
        {0, 0, 0, 1}
    };
    return rotation;
}

template<class T>
glm::mat4 BaseObject<T>::getPositionMat4(glm::vec3 position) const
{
    glm::mat4 matrix(1.0f);
    return glm::translate(matrix, position);
}

template<class T>
glm::mat4 BaseObject<T>::getScaleMat4(glm::vec3 scale) const
{
    // identity
    glm::mat4 matrix(1.0f);
    matrix[0][0] *= scale.x;
    matrix[1][1] *= scale.y;
    matrix[2][2] *= scale.z;

    return matrix;
}

template<class T>
glm::mat4 BaseObject<T>::getScaleMat4(float scale) const
{
    glm::mat4 matrix(1.0f);
    matrix[0][0] *= scale;
    matrix[1][1] *= scale;
    matrix[2][2] *= scale;
    return matrix;
}

template<class T>
float BaseObject<T>::GetDistanceFromCamera() const
{
    Camera * cam = Camera::getInstance();
    return glm::length(cam->Position-position);
}





// Builders
template<class T>
T* BaseObject<T>::SetPosition(glm::vec3 position_in)
{
    position = position_in;
    return static_cast<T*>(this);
}

template<class T>
T* BaseObject<T>::SetRotation(glm::vec3 rotation_in)
{
    rotation = rotation_in;
    return static_cast<T*>(this);
}



template<class T>
T* BaseObject<T>::SetScale(float scale_in)
{
    scaleScalar = scale_in;
    return static_cast<T*>(this);
}

template<class T>
T* BaseObject<T>::SetIsVisible(bool toggle)
{
    isVisible = toggle;
    return static_cast<T*>(this);
}
