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
// temp test with randomness

// IMGUI
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

// City_gen
#include <shader.hpp> // Custom shader header
#include <camera.hpp> // Camera class
#include <resourceManager.hpp>
#include <inputHandler.hpp>
#include <menues.hpp>
#include <scene.hpp>
#include <renderer.hpp>
#include <generator.hpp>
#include <road.hpp>

// Prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

void joystick_callback(int jid, int event); 

// Window constatns
const unsigned int WINDOW_WIDTH = 1280;
const unsigned int WINDOW_HEIGHT = 720;
const char* windowTitle = "City Generator";

// // Camera
Camera* camera = Camera::getInstance(glm::vec3{0.0f, 0.0f, 3.0f});

// Resource manager
ResourceManager* RM = ResourceManager::getInstance();

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
    LOG(STATUS, "Started");
    // Initalise GLFW for the window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // Spec what the window should have
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); // Setting to version 4.6
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    

    glfwWindowHint(GLFW_SAMPLES, 4); // MSAA x4
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

    // Framerate cap disable
    glfwSwapInterval(0);

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
    
    // int present = glfwJoystickPresent(GLFW_JOYSTICK_1);

    // Initalise GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

       // Z buffer for displaying correct trianges
    glEnable(GL_DEPTH_TEST);

    // To enable the alpha part of the color.
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    glEnable(GL_BLEND);

    glEnable(GL_MULTISAMPLE); // Toggled MSAA

    
    Scene* scene = Scene::getInstance();


    std::string preLoadAssetFilePath = "../assets/PreLoadList.txt";
    ResourceManager::getInstance()->PreLoadAssets(preLoadAssetFilePath);

    // scene->CreateSkyBox(&skyBoxImages);
    ////////////////////////
    Renderer::GetInstance()->SetClearScreenColour(BLACK);

    ShaderPath backpackShader{"../assets/shaders/backpack/vertexShader.vs", "../assets/shaders/backpack/fragmentShader.fs"};

    Shader missing("../assets/shaders/default/missing.vert", "../assets/shaders/default/missing.frag");
    std::string terrain = "../assets/models/Terrain/Terrain_first.obj";

    std::string backPathVertShader = "../assets/shaders/backpack/vertexShader.vs";
    std::string backPathFragShader = "../assets/shaders/backpack/fragmentShader.fs";

    // // GOOD POSITION TERRAIN, USE AGAIN
    scene->addTerrain(terrain, &backpackShader)
        ->SetModelOriginCenterBottom()
        ->ShowBoundingBox(false)
        ->SetLightingEnabled(true)
        ->SetIsVisible(true)
        ->IsSelectable(false)
        ->SetPosition({0,-0.5, 0})
        ->SetScale(2.5)
        ->SetTextureScale({20.0f, 20.0f});


    scene->addDirectionalLight()
        ->SetDirection(glm::vec3(0, -6, -5));

    //
    // // PERFORMANCE Testing
    //
    // // Set camera position
    // Camera::getInstance()->Position = glm::vec3{400, 200, 400};
    // scene->GetShowTerrainImGui() = false;
    // scene->GetShowSceneAxisImGui() = false;
    // scene->GetShowSkyBoxImGui() = false;
    //
    // // Buildings
    // std::array<std::string, 9> buildingModelPaths = {
    //     "../assets/models/Buildings/LowPoly/low_buildingA.obj",
    //     "../assets/models/Buildings/LowPoly/low_buildingB.obj",
    //     "../assets/models/Buildings/LowPoly/low_buildingC.obj",
    //     "../assets/models/Buildings/LowPoly/low_buildingD.obj",
    //     "../assets/models/Buildings/LowPoly/low_buildingE.obj",
    //     "../assets/models/Buildings/LowPoly/low_buildingF.obj",
    //     "../assets/models/Buildings/LowPoly/low_buildingG.obj",
    //     "../assets/models/Buildings/LowPoly/low_buildingH.obj",
    //     "../assets/models/Buildings/LowPoly/low_buildingI.obj"
    // };
    //
    // ShaderPath buildingShaderInstanced = {paths::building_defaultInstancedVertShaderPath, paths::building_defaultFragShaderPath}; 
    // ShaderPath buildingShader = {paths::building_defaultVertShaderPath, paths::building_defaultFragShaderPath};
    //
    // // const int numberOfBuildings = 100;
    // // for (int i = 0; i < numberOfBuildings; i++)
    // // {
    // //     for (int j = 0; j < 100; j++)
    // //     {
    // //         glm::vec3 position = {-200 + i*2, 0, -200 + j*2};
    // //
    // //         scene->addModel(buildingModelPaths[i%9], &buildingShader, false)
    // //             ->SetOriginFrontLeft()
    // //             ->SetPosition(position)
    // //             ->ShowBoundingBox(false)
    // //             ->SetLightingEnabled(true);
    // //     }
    // // }
    // // scene->ForceReloadInstanceRendererData();
    // // 
    // 
    // const int numberOfRoads = 1000;
    // for (int i = 0; i < numberOfRoads; i++)
    // {
    //     for (int j = 0; j < 400; j++)
    //     {
    //     glm::vec3 posA = {-200 + i*2, 0, -200+ j*2};
    //     glm::vec3 posB = {-200 + i*2 - 1, 0, -200 + j*2 -1};
    //
    //     scene->addRoad(posA, posB)
    //         ->SetWidth(0.5f);
    //     }
    //
    // }
    //
    //
    //
    // IMGUI test
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");


        
    // =============== POST-PROCESSING ===================
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Testing fbo
    // GLuint fbo;
    // glGenFramebuffers(1, &fbo);

    // float factor = 20.0f;
    // int WINDOW_WIDTH_REDUCED = WINDOW_WIDTH / factor;
    // int WINDOW_HEIGHT_REDUCED = WINDOW_HEIGHT / factor;

    // GLuint texture;
    // glGenTextures(1, &texture);
    // glBindTexture(GL_TEXTURE_2D, texture);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WINDOW_WIDTH_REDUCED, WINDOW_HEIGHT_REDUCED, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    // // Check if the framebuffer is complete
    // if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    //     // Handle error
    // }

    // // Attach the default framebuffer's color buffer to the FBO
    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);

    // // Reset framebuffer binding
    // glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Update all road buffers in the renderer
    Scene::getInstance()->roadBatchRenderer->UpdateAll();

    // bool show_demo_window = true;
    // Render loop to keep rendering until the program is closed
    // If GLFW has been instructed to close then run this function
    while (!glfwWindowShouldClose(window)){

        double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();

        // Call the input processer each loop to check if the esc key is pressed
        InputHandler::process(window, deltaTime);
        

        // =============== POST-PROCESSING ===================
        // // Bind the FBO
        // glBindFramebuffer(GL_READ_FRAMEBUFFER, 0); // Bind the default framebuffer as the source
        // glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo); // Bind your FBO as the destination
        // ===================================================

        Renderer::GetInstance()->ClearScreen();         

        scene->GetRoadObjects();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 2 View types of view, perspective and orthographic projections. We use perspective becuase we are human and have 2 eyes and so can measure depth
        //glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f); // Makes stuff look 2d
        // FOV, aspect ratio (width/height), near distance, far distance
        

        // Get the window width and height each frame so that we dont skew the image
        int windowHeight, windowWidth;
        glfwGetFramebufferSize(window, &windowWidth, &windowHeight);

        // Update cameras window paramaters
        camera->UpdateWindowDimentions(windowWidth, windowHeight);

        Menues::display(deltaTime);
        
        scene->DrawScene();

        // =============== POST-PROCESSING ===================
        // glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, WINDOW_WIDTH_REDUCED, WINDOW_HEIGHT_REDUCED, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        // Unbind the FBO
        //////////////////////////////////////////////////////


        // ImGui::ShowDemoWindow();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Perform the blit to copy from the default framebuffer to your FBO


        // Will swap the colour buffers (2d buffer that contains colour values for each pixel in GLFW window)
        glfwSwapBuffers(window);
    }

    ResourceManager::deleteInstance();

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

    if (!io.WantCaptureMouse && !InputHandler::GetShowMouse())
        InputHandler::mouse_callback_process(window, xposIn, yposIn, lastX, lastY, firstMouse);
    else{
        // As we are tabbed out set last mouse position to the one we left it at
        lastX = xposIn;
        lastY = yposIn;
    }
}

// Credit: Function from: https://github.com/opengl-tutorials/ogl/tree/master
void ScreenPosToWorldRay(int mouseX, int mouseY,             // Mouse position, in pixels, from bottom-left corner of the window
	                     int screenWidth, int screenHeight,  // Window size, in pixels
	                     glm::mat4 ViewMatrix,               // Camera position and orientation
                         glm::mat4 ProjectionMatrix,         // Camera parameters (ratio, field of view, near and far planes)
	                     glm::vec3& out_origin,              // Ouput : Origin of the ray. /!\ Starts at the near plane, so if you want the ray to start at the camera's position instead, ignore this.
	                     glm::vec3& out_direction)           // Ouput : Direction, in world space, of the ray that goes "through" the mouse.

{

	// The ray Start and End positions, in Normalized Device Coordinates (Have you read Tutorial 4 ?)
	glm::vec4 lRayStart_NDC(
		((float)mouseX/(float)screenWidth  - 0.5f) * 2.0f, // [0,1024] -> [-1,1]
		((float)mouseY/(float)screenHeight - 0.5f) * 2.0f, // [0, 768] -> [-1,1]
		-1.0, // The near plane maps to Z=-1 in Normalized Device Coordinates
		1.0f
	);
	glm::vec4 lRayEnd_NDC(
		((float)mouseX/(float)screenWidth  - 0.5f) * 2.0f,
		((float)mouseY/(float)screenHeight - 0.5f) * 2.0f,
		0.0,
		1.0f
	);


	// The Projection matrix goes from Camera Space to NDC.
	// So inverse(ProjectionMatrix) goes from NDC to Camera Space.
	glm::mat4 InverseProjectionMatrix = glm::inverse(ProjectionMatrix);
	
	// The View Matrix goes from World Space to Camera Space.
	// So inverse(ViewMatrix) goes from Camera Space to World Space.
	glm::mat4 InverseViewMatrix = glm::inverse(ViewMatrix);
	
	glm::vec4 lRayStart_camera = InverseProjectionMatrix * lRayStart_NDC;    lRayStart_camera/=lRayStart_camera.w;
	glm::vec4 lRayStart_world  = InverseViewMatrix       * lRayStart_camera; lRayStart_world /=lRayStart_world .w;
	glm::vec4 lRayEnd_camera   = InverseProjectionMatrix * lRayEnd_NDC;      lRayEnd_camera  /=lRayEnd_camera  .w;
	glm::vec4 lRayEnd_world    = InverseViewMatrix       * lRayEnd_camera;   lRayEnd_world   /=lRayEnd_world   .w;


	// Faster way (just one inverse)
	//glm::mat4 M = glm::inverse(ProjectionMatrix * ViewMatrix);
	//glm::vec4 lRayStart_world = M * lRayStart_NDC; lRayStart_world/=lRayStart_world.w;
	//glm::vec4 lRayEnd_world   = M * lRayEnd_NDC  ; lRayEnd_world  /=lRayEnd_world.w;


	glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
	lRayDir_world = glm::normalize(lRayDir_world);


	out_origin = glm::vec3(lRayStart_world);
	out_direction = glm::normalize(lRayDir_world);
}

#define RELEASE_MOUSE_BUTTON 0
#define PRESS_MOUSE_BUTTON 1
#define LEFT_MOUSE_BUTTON 0
#define RIGHT_MOUSE_BUTTON 1
#define MIDDLE_MOUSE_BUTTON 2

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    ImGuiIO& io = ImGui::GetIO();
    io.AddMouseButtonEvent(button, action);

    auto camera = Camera::getInstance();
    
    // This should not be effected by the capture of ImGui
    if (action == PRESS_MOUSE_BUTTON && button == RIGHT_MOUSE_BUTTON)
    {
        InputHandler::ToggleShowMouse();
    }

    // If we are interacting with the imgui menu then we dont want to test for another mouse hit
    if (io.WantCaptureMouse || io.WantCaptureKeyboard)
        return;
    
    

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    glm::vec3 outOrigin;
    glm::vec3 outDirection;

    if (action == PRESS_MOUSE_BUTTON && button == LEFT_MOUSE_BUTTON && InputHandler::GetShowMouse())
    {
        ScreenPosToWorldRay(xpos, camera->GetWindowHeight() - ypos, camera->GetWindowWidth(), camera->GetWindowHeight(),
                            camera->GetViewMatrix(), camera->GetProjectionMatrix(), outOrigin, outDirection);
       
        // Test by adding a line
        // glm::vec3 finalPos = outDirection * 10.0f;
        if (Scene::getInstance()->CheckForIntersection(outOrigin, outDirection))
        {
            // Scene::getInstance()->addLine(outOrigin, outOrigin+finalPos)->SetColour(GREEN);
        }
        else {
            // Scene::getInstance()->addLine(outOrigin, outOrigin+finalPos)->SetColour(RED);
        }
    }
    
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    // TODO handle imgui callback
    ImGuiIO& io = ImGui::GetIO();
    io.AddMouseWheelEvent(xoffset, yoffset);

    // Disable scroll when using gui
    if (!io.WantCaptureMouse && !InputHandler::GetShowMouse())
        InputHandler::scroll_callback_process(window, xoffset, yoffset);
}

// TODO could change the width and height with this callback function
// Callback funtion when the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

