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
#include <memory>
// temp test with randomness
#include <random>

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

// Perlin noise generator
#include <Reputeless/PerlinNoise.hpp>


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

    // Shader ourShader("assets/shaders/texture/shader1.vs", "assets/shaders/texture/shader1.fs");
    Shader backpackShader("../assets/shaders/backpack/vertexShader.vs", "../assets/shaders/backpack/fragmentShader.fs");
    // Shader ratShader("assets/shaders/square/vertexShader.vert", "assets/shaders/square/fragmentShader.frag");
    Shader missing("../assets/shaders/default/missing.vert", "../assets/shaders/default/missing.frag");

    // 1,2 are width. 3,4 are height. 5,6 are near and far plane distance

    // Z buffer for displaying correct trianges
    glEnable(GL_DEPTH_TEST);

    // To enable the alpha part of the color.
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    glEnable(GL_BLEND);

    glEnable(GL_MULTISAMPLE); // Toggled MSAA

    
    Scene* scene = Scene::getInstance();

    std::vector<std::string> skyBoxImages;
    
    skyBoxImages.push_back("../assets/textures/skybox/cloudy/bluecloud_ft.jpg");
    skyBoxImages.push_back("../assets/textures/skybox/cloudy/bluecloud_bk.jpg");
    
    skyBoxImages.push_back("../assets/textures/skybox/cloudy/bluecloud_up.jpg");
    skyBoxImages.push_back("../assets/textures/skybox/cloudy/bluecloud_dn.jpg");

    skyBoxImages.push_back("../assets/textures/skybox/cloudy/bluecloud_rt.jpg");
    skyBoxImages.push_back("../assets/textures/skybox/cloudy/bluecloud_lf.jpg");

    scene->CreateSkyBox(&skyBoxImages);


    std::string modelPath = "../assets/models/Box/cube.obj";
    std::string backPackPath = "../assets/models/Backpack/backpack.obj";
    std::string spritePath = "../assets/textures/missing.jpg";
    std::string gordon = "../assets/textures/gordon_gosling.png";
    std::string nighthawks = "../assets/textures/nighthawks.png";
    std::string grass = "../assets/textures/grass.png";
    std::string grass2 = "../assets/textures/grass2.png";
    std::string grassTexture = "../assets/textures/grassTexture.png";
    std::string cube = "../assets/models/Cube/cube.obj";
    std::string dust2 = "../assets/models/de_dust2/de_dust2.obj";

    std::string terrain = "../assets/models/Terrain/Terrain_first.obj";
    std::string tree = "../assets/textures/tree_2_cropped.png";

    std::string building1 = "../assets/models/Buildings/NoTextureStarter/CellPhoneBuilding_01.obj";
    std::string building2 = "../assets/models/Buildings/NoTextureStarter/LargeRectangle.obj";
    std::string building3 = "../assets/models/Buildings/NoTextureStarter/AngledRoofHouse.obj";

    scene->addModel(building1, nullptr)
        ->SetModelOriginCenterBottom()
        ->SetPosition(glm::vec3{10, 0, 0})
        ->ShowBoundingBox(false);
        


    scene->addModel(building2, nullptr)
        ->SetModelOriginCenterBottom()
        ->SetPosition(glm::vec3{10, 0, 10})
        ->ShowBoundingBox(false);


    scene->addModel(building3, nullptr)
        ->SetModelOriginCenterBottom()
        ->SetPosition(glm::vec3{10, 0, -10})
        ->ShowBoundingBox(false);


    scene->addModel(terrain, nullptr)
        ->SetModelOriginCenterBottom()
        ->ShowBoundingBox(false);

    scene->addSprite(tree, nullptr)
        ->SetModelOriginCenterBottom()
        ->SetIsVisible(true)
        ->SetIsBillboard(true)
        ->SetPosition(glm::vec3{0, 0, 1})
        ->SetScale(2.0f);
    scene->addSprite(tree, nullptr)
        ->SetModelOriginCenterBottom()
        ->SetIsVisible(true)
        ->SetIsBillboard(true)
        ->SetPosition(glm::vec3{0, 0, 3})
        ->SetScale(2.0f);
    scene->addSprite(tree, nullptr)
        ->SetModelOriginCenterBottom()
        ->SetIsVisible(true)
        ->SetIsBillboard(true)
        ->SetPosition(glm::vec3{0, 0, 5})
        ->SetScale(2.0f);
    scene->addSprite(tree, nullptr)
        ->SetModelOriginCenterBottom()
        ->SetIsVisible(true)
        ->SetIsBillboard(true)
        ->SetPosition(glm::vec3{0, 0, 7})
        ->SetScale(2.0f);



    // const siv::PerlinNoise::seed_type seed = 123456u;
	// const siv::PerlinNoise perlin{ seed };
	// for (int y = 0; y < 160; ++y)
	// {
	// 	for (int x = 0; x < 160; ++x)
	// 	{
	// 		const double noise = perlin.noise2D_01((x), (y));

    //         if (noise > 0.7)
    //         {
    //             scene->addModel(building1, nullptr)
    //                 ->SetModelOriginCenterBottom()
    //                 ->SetPosition(glm::vec3{(x-80)*2, 0, (y-80)*2})
    //                 ->ShowBoundingBox(false);
    //         }
	// 	}
	// }


    // Note for terrain generation the terrain asset is 160 by 160


    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(-160, 160); // define the range

    for (int i = 0; i < 100; i++)
    {
        scene->addSprite(tree, nullptr)
            ->SetModelOriginCenterBottom()
            ->SetIsVisible(true)
            ->SetIsBillboard(true)
            ->SetPosition(glm::vec3{static_cast<float>(distr(gen)), 0, static_cast<float>(distr(gen))})
            ->SetScale(2.0f);
    }

    scene->addModel(backPackPath, nullptr)
         ->SetModelOriginCenter()
         ->SetPosition(glm::vec3{0.0f, 1.0f, 0.0f})
         ->ShowBoundingBox(false);





    // X Y Z (R G B) Lines for the orientation
    // X is Red
    scene->addLine(nullptr, glm::vec3{-1000.0, 0, 0}, glm::vec3{1000.0, 0, 0}, glm::vec3{1, 0, 0});
    // Y is Green
    scene->addLine(nullptr, glm::vec3{0, -1000.0, 0}, glm::vec3{0, 1000.0, 0}, glm::vec3{0, 1, 0});
    // Z is Blue
    scene->addLine(nullptr, glm::vec3{0, 0, -1000.0}, glm::vec3{0, 0, 1000.0}, glm::vec3{0, 0, 1});

    // IMGUI test
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    // bool show_demo_window = true;
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

        // 2 View types of view, perspective and orthographic projections. We use perspective becuase we are human and have 2 eyes and so can measure depth
        //glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f); // Makes stuff look 2d
        // FOV, aspect ratio (width/height), near distance, far distance
        glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 1000.0f);
        glm::mat4 view = camera->GetViewMatrix();
        
        Menues::display(deltaTime);
        
        // Remove translation from matrix by casting to mat3 then mat4
        glm::mat4 viewSB = glm::mat4(glm::mat3(camera->GetViewMatrix()));
        scene->DrawSkyBox(viewSB, projection);
        scene->drawSceneObjects(view, projection);
        
        


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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
