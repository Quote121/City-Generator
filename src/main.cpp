///////////////////////////////
//// Definitions
//
// Fragment - in openGL a fragment is all the data required to render a single pixel
//
// Fragment shader - a shader to calculate the final colour of a pixel eg lights shadows colour of the light etc
// 
// Normalized device coords - -1 to 1 values relative to the window
// 
// VBO - vertex buffer objects (They can store a large number of verticies in the GPU's memory
//
// VAO - Vertex Array objects
// 
// EBO - Element buffer object
// 
///////////////////////////////

// Glad has to be before glfw as it contains certain headers that will clash otherwise
// Glad/GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp> // opengl mathematics
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// STD
#include <stdlib.h> // Clearing cmd for fps
#include <iostream>
#include <cmath> 

// IMGUI
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <memory>

// Other
#include "shader.hpp" // Custom shader header
#include "stb_image/stb_image.h" // Image imports
#include "camera.hpp" // Camera class
#include "texture.hpp"
#include "inputHandler.hpp"

#include "menues.hpp"
#include "scene.hpp"

// Prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

void joystick_callback(int jid, int event);

// Window constatns
const unsigned int WINDOW_WIDTH = 1280;
const unsigned int WINDOW_HEIGHT = 720;
const char* windowTitle = "OpenGL learning";

// // Camera
Camera* camera = Camera::getInstance(glm::vec3{0.0f, 0.0f, 3.0f});

// Mouse position data (center of the screen by init
float lastX = WINDOW_WIDTH / 2.0f, lastY = WINDOW_HEIGHT / 2.0f;
bool firstMouse = true;

// Frame vars
double deltaTime = 0.0; // Time between current frame and last frame
double lastFrame = 0.0; // Time of last frame

// Deadzones for controller joysticks
float InputHandler::x_axis_deadzone = 0.3;
float InputHandler::y_axis_deadzone = 0.3;


int main() {
    // Initalise GLFW for the window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // Spec what the window should have
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); // Setting to version 4.6
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // MACOSX
    #endif

    // PAGE 20

    // Create the window assign it to var window
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, windowTitle, NULL, NULL);
    // ViewPort tell openGL the size of the rendering window
    // First 2 params are where the window will be created

    // Create before callbacks as they use it
    // Get the only instance of camera
    
    if (window == NULL){
        // Error
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // Telling open gl that we wanth the resize callback funtion to be called on window when its resized
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Hide cursor and capture its input
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);

    //////////////////
    //
    // Gamepad
    //
    //////////////////
    int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
    std::cout << "Present" << present << std::endl;
    glfwSetJoystickCallback(joystick_callback);

    // glfwSwapInterval(0); // This will disable vsync and remove frame rate cap
    

    // Initalise GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    Shader ourShader("assets/shaders/texture/shader1.vs", "assets/shaders/texture/shader1.fs");
    Shader backpackShader("assets/shaders/backpack/vertexShader.vs", "assets/shaders/backpack/fragmentShader.fs");
    Shader ratShader("assets/shaders/square/vertexShader.vert", "assets/shaders/square/fragmentShader.frag");
    Shader missing("assets/shaders/default/missing.vert", "assets/shaders/default/missing.frag");

    // 1,2 are width. 3,4 are height. 5,6 are near and far plane distance

    // Z buffer for displaying correct trianges
    glEnable(GL_DEPTH_TEST);

    // To enable the alpha part of the color.
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    glEnable(GL_BLEND);

    // Removes the backfaces of faces
    // however the cubes in the current state are not correct, normals wrong way
    glEnable(GL_CULL_FACE);


    Scene* scene = Scene::getInstance();
    scene->addObject("assets/models/ShippingContainer/container.obj", backpackShader);
    scene->addObject("assets/models/Backpack/backpack.obj", backpackShader);
    scene->addObject("assets/models/Rat/rat.obj", ratShader);
    // scene->addObject("assets/models/ShippingContainer/container.obj", backpackShader);



    // IMGUI test
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    bool show_demo_window = true;
    // Render loop to keep rendering until the program is closed
    // If GLFW has been instructed to close then run this function
    while (!glfwWindowShouldClose(window)){

        double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();

        // ImGui::ShowDemoWindow();

        // Call the input processer each loop to check if the esc key is pressed
        InputHandler::process(window, deltaTime);
        
        // Clearing colour buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        /////////////
        // FPS Count
        /////////////
        static int fpsSampCount = 0;
        static float fpsSum = 0;
        // Print fps and coords
        const int sampleSize = 100;
        float fpsValue;
        fpsSum += (1 / deltaTime);
        fpsSampCount += 1;
        if (fpsSampCount == sampleSize) {
            fpsValue = ceil((int)(fpsSum / sampleSize * 10)) / 10;
            fpsSum = 0; fpsSampCount = 0;
        }

        // 2 View types of view, perspective and orthographic projections. We use perspective becuase we are human and have 2 eyes and so can measure depth
        //glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f); // Makes stuff look 2d
        // FOV, aspect ratio (width/height), near distance, far distance
        glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 1000.0f);
        glm::mat4 view = camera->GetViewMatrix();
        
        // static float speed = 10.0f;
        // static float col[4] = {0.5f, 0.5f, 0.5f, 1.0f};

        
        Menues::display(camera);
        // Menues::test();
        scene->drawObjects(view, projection);


        // own scope for imgui idk why, lookinto it 
        {
        ImGui::Begin("Stats");
        std::stringstream imGuiFPS;
        imGuiFPS << "FPS: " << fpsValue;
        ImGui::Text(imGuiFPS.str().c_str());
        std::stringstream viewss;
        viewss << "Pitch | Yaw: " << camera->Pitch << " " << camera->Yaw;
        ImGui::Text(viewss.str().c_str());
        std::stringstream pos_ss;
        pos_ss << "[x,y,z] : " << camera->GetPositionCoords();
        ImGui::Text(pos_ss.str().c_str());
        ImGui::End();
        }


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Will swap the colour buffers (2d buffer that contains colour values for each pixel in GLFW window)
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    // Context destruction

    // Delete all GLFW resources that where allocated before
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


// Callback for when a devide such as a gamepad is used
void joystick_callback(int jid, int event){
    InputHandler::joystick_callback_process(jid, event);
}

// Process mouse inputs // xpos and ypos origignally doubles 
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    ImGuiIO& io = ImGui::GetIO();
    io.AddMousePosEvent(xposIn, yposIn);

    if (!io.WantCaptureMouse && !InputHandler::showMouse)
        InputHandler::mouse_callback_process(window, xposIn, yposIn, lastX, lastY, firstMouse);
    else{
        // As we are tabbed out set last mouse position to the one we left it at
        lastX = xposIn;
        lastY = yposIn;
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    ImGuiIO& io = ImGui::GetIO();
    io.AddMouseButtonEvent(button, action);

    // if (!io.WantCaptureMouse)
    // mouseButtonCallback (We dont handle mouse buttons yet)
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    // TODO handle imgui callback
    ImGuiIO& io = ImGui::GetIO();
    io.AddMouseWheelEvent(xoffset, yoffset);

    // Disable scroll when using gui
    if (!io.WantCaptureMouse && !InputHandler::showMouse)
        InputHandler::scroll_callback_process(window, xoffset, yoffset);
}

// Callback funtion when the function is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}