#include "inputHandler.hpp"
#include <imgui/imgui.h>

#include <iostream>

bool InputHandler::showMouse = false;

bool InputHandler::getJoyStick()
{
    for (unsigned int i = 0; i < GLFW_JOYSTICK_LAST; i++)
    {
        if (glfwJoystickIsGamepad(GLFW_JOYSTICK_1+i))
        {
            this->joystick.jid = i;
            this->joystick.name = (char*)glfwGetGamepadName(i);
            this->joystick.connected = true;
            
            return true;
        }

    }
    // Not found
    return false;
}

void InputHandler::joystick_callback_process(int jid, int event)
{
    if (event == GLFW_CONNECTED)
    {
        std::cout << "\nJoystick/Gamepad [" << jid << "] was connected.";
    }
    else
    {
        std::cout << "\nJoystick/Gamepad [" << jid << "] was disconnected.";
    }
}



void InputHandler::mouse_callback_process(GLFWwindow *window, double xposIn, double yposIn, float& lastX, float& lastY, bool& firstMouse)
{
#ifdef MOUSE_CALLBACK_OUTPUT
    std::cout << 
        "XposIn: " << xposIn <<
        "| YposIn: " << yposIn <<
        "| lastX: " << lastX <<
        "| lastY: " << lastY << std::endl;
#endif // MOUSE_CALLBACK_OUTPUT
    Camera* camera = Camera::getInstance();

    // Cast xpos to float
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    // First time tabbing in
    // This is only relevent when the program first runs and captures the mouse input from anypart of the screen
    // then recentering it will cuase a large difference and large movement
    if (firstMouse) // initially set to true
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed y ranges bottom to top
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;



    camera->ProcessMouseMovement(xoffset, yoffset);
}

void InputHandler::scroll_callback_process(GLFWwindow *window, double xoffset, double yoffset)
{
    Camera* camera = Camera::getInstance();
    camera->ProcessMouseScroll(static_cast<float>(yoffset));
}

void InputHandler::process(GLFWwindow *window, double deltaTime)
{
    Camera* camera = Camera::getInstance();


    // For gamepad // keyboard will still work
    if (glfwJoystickIsGamepad(GLFW_JOYSTICK_1))
    {
        GLFWgamepadstate state;

#ifdef JOYSTICK_INPUT_DEBUG_READOUT
            std::cout << "Left X: " << state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] << 
                        " Left Y: " << state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] << 
                        " || Right X: " << state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X] <<
                        " Left Y: " << state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y] << 
                        " || LEFT TRIGGER: " << state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] <<
                        " RIGHT TRIGGER: " << state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] <<
                        std::endl;;
#endif


        if (glfwGetGamepadState(GLFW_JOYSTICK_1, &state))
        {
            // Quit
            if (state.buttons[GLFW_GAMEPAD_BUTTON_BACK])
            {
                std::cout << "\nGLFW_GAMEPAD_BUTTON_BACK was pressed. Closing..." << std::endl;
                glfwSetWindowShouldClose(window, true);
            }

            // Up and down with bumpers
            if (state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER])
                camera->processKeyboard(Camera_Movement::DOWN, deltaTime);

            if (state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER])
                camera->processKeyboard(Camera_Movement::UP, deltaTime);

            // Zoom
            camera->ProcessMouseScroll((state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER]+1) * -1);
            camera->ProcessMouseScroll(state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER]+1);


            // Translation movement left and right
            if (state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] > x_axis_deadzone)
                camera->processKeyboard(Camera_Movement::RIGHT, deltaTime);
            if (state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] < -x_axis_deadzone)
                camera->processKeyboard(Camera_Movement::LEFT, deltaTime);

            // Translation movement back and forwards
            if (state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] > y_axis_deadzone)
                camera->processKeyboard(Camera_Movement::BACKWARD, deltaTime);
            if (state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] < -y_axis_deadzone)
                camera->processKeyboard(Camera_Movement::FORWARD, deltaTime);

            // SPRINT
            if (state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_THUMB])
            {
                if (state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] < -y_axis_deadzone)
                    camera->processKeyboard(Camera_Movement::FORWARD, deltaTime, 2.0f);

                if (state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] > y_axis_deadzone)
                    camera->processKeyboard(Camera_Movement::BACKWARD, deltaTime, 2.0f);
            }
            
            // Look // TODO have a look tied to joystick as speed is dependent on framerate
            if (glm::abs(state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X]) > x_axis_deadzone || glm::abs(state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y]) > y_axis_deadzone)
                camera->ProcessMouseMovement(state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X], -state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y]);
        }
    }

    // Keyboard

    // Exit
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        std::cout << "\nGLFW_KEY_ESCAPE was pressed. Closing..." << std::endl;
        glfwSetWindowShouldClose(window, true);
    }

    // Movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->processKeyboard(Camera_Movement::FORWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->processKeyboard(Camera_Movement::BACKWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->processKeyboard(Camera_Movement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->processKeyboard(Camera_Movement::RIGHT, deltaTime);

    // Movement - Sprint
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera->processKeyboard(Camera_Movement::FORWARD, deltaTime, 2.0f);

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera->processKeyboard(Camera_Movement::BACKWARD, deltaTime, 2.0f);
    }

    // Movement - up and down
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera->processKeyboard(Camera_Movement::UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera->processKeyboard(Camera_Movement::DOWN, deltaTime);

    
    // For toggling mouse visibility
    // showMouse flag which is a static bool of inputHandler tells mouse callback to 
    // redirect input to imgui intead of here
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        showMouse = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {   
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        showMouse = false;
    }
}
