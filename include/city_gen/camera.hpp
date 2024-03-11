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
	static Camera* getInstance(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	static Camera* getInstance(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);


    // inline bool GetFreeModeMode(void) const
    // {
    //     return freeMouseMode;
    // }
    //
    // inline void SetFreeMouseMode(bool mode)
    // {
    //     this->freeMouseMode = mode;
    // }
    //
    // inline void ToggleFreeMouseMode(void)
    // {
    //     this->freeMouseMode = !this->freeMouseMode;
    //     InputHandler::SetShowMouse(freeMouseMode);
    // }

	// returns the view matrix calculated using Euler Angles and the LookAt Matrix
	inline glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

    inline glm::mat4 GetProjectionMatrix()
    {
        return glm::perspective(glm::radians(this->Zoom), (float)windowWidth / (float)windowHeight, 0.1f, 10000.0f);
    }

    // Get window width and height
    inline int GetWindowWidth(void) const
    {
        return windowWidth;
    }

    inline int GetWindowHeight(void) const
    {
        return windowHeight;
    }

    void UpdateWindowDimentions(const int width, const int height)
    {
        windowWidth = width;
        windowHeight = height;
    }

	// Process keyboard input // Speed multiplier for movemnt speed increase with shift key
	void processKeyboard(Camera_Movement direction, float deltaTime, float speedMultiplier = 1) {

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

	// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
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

	// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(float yoffset)
	{
		Zoom -= yoffset;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 45.0f)
			Zoom = 45.0f;
	}

    glm::vec3& GetPositionHandle(void)
    {
        return Position;
    }

private:
	// Private the default constructor
	Camera() = default;
	// Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}
	// constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	~Camera() { }

	// Singleton
	static Camera *pinstance_;

	Camera(const Camera&) = delete;
	Camera& operator=(const Camera&) = delete;

	// calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors();
};

