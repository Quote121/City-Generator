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


    // Skybox creation ////
    std::vector<std::string> skyBoxImages;
    
    skyBoxImages.push_back("../assets/textures/skybox/cloudy/bluecloud_ft.jpg");
    skyBoxImages.push_back("../assets/textures/skybox/cloudy/bluecloud_bk.jpg");

    skyBoxImages.push_back("../assets/textures/skybox/cloudy/bluecloud_up.jpg");
    skyBoxImages.push_back("../assets/textures/skybox/cloudy/bluecloud_dn.jpg");

    skyBoxImages.push_back("../assets/textures/skybox/cloudy/bluecloud_rt.jpg");
    skyBoxImages.push_back("../assets/textures/skybox/cloudy/bluecloud_lf.jpg");

    // Doesn't work, havent investigated
    // skyBoxImages.push_back("../assets/textures/skybox/night/starmap_8k.jpg");
    // skyBoxImages.push_back("../assets/textures/skybox/night/starmap_8k.jpg");
    // skyBoxImages.push_back("../assets/textures/skybox/night/starmap_8k.jpg");
    // skyBoxImages.push_back("../assets/textures/skybox/night/starmap_8k.jpg");
    // skyBoxImages.push_back("../assets/textures/skybox/night/starmap_8k.jpg");
    // skyBoxImages.push_back("../assets/textures/skybox/night/starmap_8k.jpg");



    scene->CreateSkyBox(&skyBoxImages);
    ////////////////////////
    Renderer::GetInstance()->SetClearScreenColour(BLACK);


    ShaderPath backpackShader{"../assets/shaders/backpack/vertexShader.vs", "../assets/shaders/backpack/fragmentShader.fs"};

    Shader missing("../assets/shaders/default/missing.vert", "../assets/shaders/default/missing.frag");

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

    // std::string terrain = "../assets/models/Terrain/Terrain_repeat.obj";
    std::string tree = "../assets/textures/tree_2_cropped.png";
    std::string sunIcon = "../assets/textures/sun_icon.png";

    std::string roadMdl = "../assets/models/demo_01/road/road_asphalt.obj";

    std::string building1 = "../assets/models/Buildings/NoTextureStarter/CellPhoneBuilding_01.obj";
    std::string building2 = "../assets/models/Buildings/NoTextureStarter/LargeRectangle.obj";
    std::string building3 = "../assets/models/Buildings/NoTextureStarter/AngledRoofHouse.obj";


 //    //
 //    auto testobj1 = scene->addModel(building1, nullptr, true)
 //        ->SetModelOriginCenterBottom()
 //        ->SetPosition(glm::vec3{10, 0, 0})
 //        ->ShowBoundingBox(false);
 //        
 //    auto testobj21 = scene->addModel(building1, nullptr, true)
 //        ->SetModelOriginCenterBottom()
 //        ->SetPosition(glm::vec3{7, 0, 0})
 //        ->ShowBoundingBox(false);
 // 
 //
 //    auto testobj2 = scene->addModel(building2, nullptr, false)
 //        ->SetModelOriginCenterBottom()
 //        ->SetPosition(glm::vec3{10, 0, 10})
 //        ->ShowBoundingBox(false);
 //    auto testObj3 = scene->addModel(building3, nullptr, false)
 //        // ->SetModelOriginCenterBottom()
 //        ->SetOriginFrontLeft()
 //        ->SetPosition(glm::vec3{0, 0, 0})
 //        ->ShowBoundingBox(true);
 //
 //    
 //    // testing instance renderer
 //    InstanceRenderer<ModelObject*>* IR = new InstanceRenderer<ModelObject*>();
 //    
 //    IR->Append(testobj1);
 //    IR->Append(testobj2);
 //    IR->Append(testObj3);
 //    IR->Remove(testobj2);
 //
 //    testobj1->SetScale(5);
 //
 //    // IR->Update(testobj1);
 //    
 //    testobj1->SetScale(2);
 //
 //    // IR->UpdateAll();


    // scene->ForceReloadInstanceRendererData();

    // IR->Draw();

    // for (int i = 0; i < 30; i++)
    // {
    //     scene->addModel(building1)
    //         ->SetPosition({i+3, 0, i+10});
    // }

    //
    std::string backPathVertShader = "../assets/shaders/backpack/vertexShader.vs";
    std::string backPathFragShader = "../assets/shaders/backpack/fragmentShader.fs";
    //
    //
    // scene->addModel(roadMdl)
    //     ->SetModelOriginCenter()
    //     ->SetPosition(glm::vec3{0,0.01,0})
    //     ->ShowBoundingBox(false);
    //     
    //
    //

    // // GOOD POSITION TERRAIN, USE AGAIN
    scene->addTerrain(terrain, &backpackShader)
        ->SetModelOriginCenterBottom()
        ->ShowBoundingBox(false)
        ->SetLightingEnabled(true)
        ->SetIsVisible(true)
        ->IsSelectable(false)
        ->SetPosition({0,-0.4, 0})
        ->SetScale(2)
        ->SetTextureScale({10.0f, 10.0f});


    // Tree asset generation
    // generator::generateRoads(3, 10.0f, 3.0f, 90.0f);


    // Tree asset generation
   //  std::random_device rd; // obtain a random number from hardware
   // 
   //  std::mt19937 gen(rd()); // seed the generator
   //  std::uniform_int_distribution<> distr(-160, 160); // define the range
   //
    // for (int i = 0; i < 100; i++)
    // {
    //     scene->addSprite(tree)
    //         ->SetModelOriginCenterBottom()
    //         ->SetIsVisible(true)
    //         ->SetIsBillboard(true)
    //         ->SetPosition(glm::vec3{static_cast<float>(distr(gen)), 0, static_cast<float>(distr(gen))})
    //         ->SetScale(2.0f);
    // }
        scene->addSprite(tree)
            ->SetModelOriginCenterBottom()
            ->SetIsVisible(false)
            ->SetIsBillboard(true)
            ->SetPosition({0, 0, 0})
            ->SetScale(2.0f);


    scene->addDirectionalLight()
        ->SetDirection(glm::vec3(0, -3.14, -0.5));

    
    Shader lineShader("../assets/shaders/default/line/line_shader.frag",
                     "../assets/shaders/default/line/line_shader.frag");

    
    // std::string buildingTest = "../assets/models/Buildings/buildingTest.obj";
    // ShaderPath buildingShader = {paths::building_defaultVertShaderPath, paths::building_defaultFragShaderPath}; 
    // scene->addModel(buildingTest, &buildingShader)
    //     ->SetLightingEnabled(true)
    //     ->SetOriginFrontLeft()
    //     ->SetPosition({1, 0, 1});
    //     // ->SetSpawnOffset();

    // Using builder rather than constructor will have larger overhead as object will be created then modified
    // Builder WILL DO a second UpdateVertices() call as it will need to recalculate based on new width
    // scene->addRoad(glm::vec3{1, 7.01, 6}, glm::vec3{12, 0.5, 6})->SetWidth(3.0f);
    // scene->addRoad(glm::vec3{1, 7.01, 12}, glm::vec3{12, 0.5, 12}, 1.0f);
   
    scene->addRoad({5, 0, 5}, {5, 0, 10}, 1.0f);
    scene->addRoad({7, 0, 10}, {7, 0, 5}, 1.0f);
    // //
    // scene->addRoad({-5, 0, 5}, {-10, 0, 5}, 1.0f);
    // scene->addRoad({-10, 0, 7}, {-5, 0, 7}, 1.0f);
    // //
    // scene->addRoad({-1, 0, -1}, {-6, 0, -6});
    // scene->addRoad({-10, 0, -12}, {-5, 0, -7});
    //


    // scene->addRoad({4, 0, 0}, {-4, 0, 0}, 1.0f); 
    // scene->addRoad({10,0, 8}, {-10,0,8}, 1.0f);

    // Wireframe for debugging
    // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );


    // Road* j = new Road(&lineShader);
    // j->UpdateVertices(glm::vec3(1, 0    , 6), glm::vec3(-20, 0, 30));

    // // BUG passing line shader reference here will result in a failure
    // LineObject* LO = new LineObject(&lineShader, glm::vec3(4, 0, 6), glm::vec3(8, 0, 13));
    // LO->SetColour(glm::vec3{0.0f, 0.0f, 1.0f});  
    
    // scene->addSprite(sunIcon, nullptr) 
    //     ->SetPosition(glm::vec3(0,0,0))
    //     ->SetModelOriginCenter()
    //     ->SetIsBillboard(true);

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
        
        scene->DrawSkyBox();
        scene->DrawSceneObjects();

        // =============== POST-PROCESSING ===================
        // glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, WINDOW_WIDTH_REDUCED, WINDOW_HEIGHT_REDUCED, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        // Unbind the FBO
        //////////////////////////////////////////////////////


        ImGui::ShowDemoWindow();

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
        glm::vec3 finalPos = outDirection * 10.0f;
        if (Scene::getInstance()->CheckForIntersection(outOrigin, outDirection))
        {

            Scene::getInstance()->addLine(outOrigin, outOrigin+finalPos)->SetColour(GREEN);
        }
        else {
        
            Scene::getInstance()->addLine(outOrigin, outOrigin+finalPos)->SetColour(RED);
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

