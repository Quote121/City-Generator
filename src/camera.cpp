#include "camera.hpp"

// Defined static member variable
Camera* Camera::pinstance_{nullptr};

Camera *Camera::getInstance(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
{
    // First time creating class
    if (pinstance_ == nullptr)
    {
        pinstance_ = new Camera(position, up, yaw, pitch);
    }

    return pinstance_;
}

Camera *Camera::getInstance(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
{
    // First time creating class
    if (pinstance_ == nullptr)
    {
        pinstance_ = new Camera(posX, posY, posZ, upX, upY, upZ, yaw, pitch);
    }

    return pinstance_;
}

void Camera::updateCameraVectors()
{
    // calculate the new front vector
    Front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front.y = sin(glm::radians(Pitch));
    Front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(Front);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = glm::normalize(glm::cross(Right, Front));
}