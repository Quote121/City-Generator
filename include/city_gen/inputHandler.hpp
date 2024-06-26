#pragma once

#include <GLFW/glfw3.h>
#include <imgui/imgui.h>

#include <camera.hpp>

// For joystick readout
//#define JOYSTICK_INPUT_DEBUG_READOUT
// For mouse pos readout
//#define MOUSE_CALLBACK_OUTPUT


struct joyStick
{
    const char* name;
    unsigned int jid;
    bool connected;
};

class InputHandler
{

private:
    static float x_axis_deadzone;
    static float y_axis_deadzone;

    joyStick joystick;

    static bool showMouse;
public:


    /*
    Returns true or false if connected
    Pass jid as ref to get the joystick reference id
    */
    bool getJoyStick();

    /* process all inputs, runs once a frame */

    static void joystick_callback_process(int jid, int event);

    static void process(GLFWwindow* window, double deltaTime);

    static void mouse_callback_process(GLFWwindow* window, double xposIn, double yposIn, float& lastX, float& lastY, bool& firstMouse);

    static void scroll_callback_process(GLFWwindow* window, double xoffset, double yoffset);

    static bool GetShowMouse(void) 
    {
        return showMouse;
    }

    static void SetShowMouse(bool state)
    {
        showMouse = state;
    }

    static void ToggleShowMouse(void)
    {
        showMouse = !showMouse;
    }
};
