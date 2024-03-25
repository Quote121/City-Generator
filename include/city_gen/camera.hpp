#pragma once

#include <GLFW/glfw3.h> // GL_Boolean etc.
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Enum class declared instead of enum as other enum types can be seen as equivilent
enum class Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.5f;
const float ZOOM = 45.0f;

/*
Camera is a singleton class
this means that we cannot create another instance of it
*/

class Camera {

private:
    // We need to know the window size for projection matrix
    int windowWidth;
    int windowHeight;

public:
	// Camera attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp; // {0.0, 1.0, 0.0}
	// euler angles
	float Yaw;
	float Pitch;
	// camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	// Singleton constructors
    // @brief Singleton constructor
    // @args position - position of camera
    // @args up - up vector of camera
    // @args yaw - up and down angle
    // @args pitch - left and right angle
    // @returns Camera* - returns this class
    static Camera* getInstance(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
                               glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
                               float yaw = YAW, 
                               float pitch = PITCH);

	static Camera* getInstance(float posX, 
                               float posY, 
                               float posZ, 
                               float upX, 
                               float upY, 
                               float upZ, 
                               float yaw, 
                               float pitch);

    // @brief gets the view matrix of the camera using euler angles and lookat matrix
	inline glm::mat4 GetViewMatrix(void) const;

    // @brief returns the projection matrix of the camera
    inline glm::mat4 GetProjectionMatrix(void) const;

    // @brief get the glfw window width
    inline int GetWindowWidth(void) const;

    // @brief get the glfw window height
    inline int GetWindowHeight(void) const;

    // @brief update the width and height parameters
    // @args width - width in px
    // @args height - height in px
    void UpdateWindowDimentions(const int width, const int height);

    // @brief Process keyboard input
    // @args direction facing
    // @args deltaTime - to stop movement being framerate dependant
    // @args speedMultiplier - pressing shift key will multiply speed by x amount
	void processKeyboard(Camera_Movement direction, float deltaTime, float speedMultiplier = 1);

    // @brief Process mouse input recived from GLFW
    // @args xoffset - change of mouse in x axis
    // @args yoffset - change of mouse in y axis
    // @args constrainPitch - stops from looking directly up or down 
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

    // @brief process input from mouse scroll wheel event
    // @args the change from glfw
	void ProcessMouseScroll(float yoffset);

    // @brief Get Position handle for direct change of camera position via imgui
    // @returns reference to the camera vec3 position
    glm::vec3& GetPositionHandle(void);
private:
    // Private constructors 
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	// Singleton
	static Camera *pinstance_;
	Camera(const Camera&) = delete;
	Camera& operator=(const Camera&) = delete;

	// calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors();
};

// Inline definitions
inline glm::mat4 Camera::GetViewMatrix(void) const
{
    return glm::lookAt(Position, Position + Front, Up);
}

inline glm::mat4 Camera::GetProjectionMatrix(void) const
{
    return glm::perspective(glm::radians(this->Zoom), (float)windowWidth / (float)windowHeight, 0.1f, 10000.0f);
}

inline int Camera::GetWindowWidth(void) const
{
    return windowWidth;
}

inline int Camera::GetWindowHeight(void) const
{
    return windowHeight;
}

