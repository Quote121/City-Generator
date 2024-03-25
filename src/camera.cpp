#include <camera.hpp>

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
    // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Right = glm::normalize(glm::cross(Front, WorldUp));  
    Up = glm::normalize(glm::cross(Right, Front));
}

Camera::Camera(glm::vec3 position, 
               glm::vec3 up, 
               float yaw, 
               float pitch) 
               : Front(glm::vec3(0.0f, 0.0f, -1.0f)), 
               MovementSpeed(SPEED), 
               MouseSensitivity(SENSITIVITY), 
               Zoom(ZOOM)
{
    this->Position = position;
    this->WorldUp = up;
    this->Yaw = yaw;
    this->Pitch = pitch;
    updateCameraVectors();
}


Camera::Camera(float posX, 
               float posY, 
               float posZ, 
               float upX, 
               float upY, 
               float upZ, 
               float yaw, 
               float pitch) 
               : Front(glm::vec3(0.0f, 0.0f, -1.0f)), 
               MovementSpeed(SPEED), 
               MouseSensitivity(SENSITIVITY), 
               Zoom(ZOOM)
{
    this->Position = glm::vec3(posX, posY, posZ);
    this->WorldUp = glm::vec3(upX, upY, upZ);
    this->Yaw = yaw;
    this->Pitch = pitch;
    updateCameraVectors();
}

void Camera::UpdateWindowDimentions(const int width, const int height)
{
    windowWidth = width;
    windowHeight = height;
}

void Camera::processKeyboard(Camera_Movement direction, float deltaTime, float speedMultiplier) {

    // Camera will move at 2.5 units per second
    float velocity = MovementSpeed * deltaTime;

    if (direction == Camera_Movement::FORWARD)
        Position += Front * velocity * speedMultiplier;
    if (direction == Camera_Movement::BACKWARD)
        Position -= Front * velocity * speedMultiplier;
    if (direction == Camera_Movement::LEFT)
        Position -= Right * velocity * speedMultiplier;
    if (direction == Camera_Movement::RIGHT)
        Position += Right * velocity * speedMultiplier;

    // Space and ctrl for up and down
    if (direction == Camera_Movement::UP)
        Position += Up * velocity;
    if (direction == Camera_Movement::DOWN)
        Position -= Up * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
    Zoom -= yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

glm::vec3& Camera::GetPositionHandle(void)
{
    return Position;
}

