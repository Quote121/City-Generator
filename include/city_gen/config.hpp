#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include <array>

#include <stopwatch.hpp>

/*
    Header for global config info such as file paths, constants, data

    Note: Paths are relative to the executable in the bin file
*/

//////////////// SHADERS ////////////////

// Sprite default shaders
namespace paths
{
extern const char* sprite_defaultVertShaderPath;
extern const char* sprite_defaultFragShaderPath;

// Object default shaders
extern const char* object_defaultVertShaderPath;
extern const char* object_defaultFragShaderPath;
extern const char* object_defaultInstancedVertShaderPath;

// Line object shaders
extern const char* line_defaultVertShaderPath;
extern const char* line_defaultFragShaderPath;

// Road shaders
extern const char* road_defaultVertShaderPath;
extern const char* road_defaultFragShaderPath;


// Bounding box shaders
extern const char* bb_defaultVertShaderPath;
extern const char* bb_defaultFragShaderPath;

// Skybox shader
extern const char* skybox_defaultVertShaderPath;
extern const char* skybox_defaultFragShaderPath;

// Building shader
extern const char* building_defaultVertShaderPath;
extern const char* building_defaultFragShaderPath;
extern const char* building_defaultInstancedVertShaderPath;

// Sun Icon for lights
extern const char* light_icon_texture;

constexpr std::array<std::string_view, 6> blueSkyBox = {
    "../assets/textures/skybox/cloudy/bluecloud_ft.jpg",
    "../assets/textures/skybox/cloudy/bluecloud_bk.jpg",
    "../assets/textures/skybox/cloudy/bluecloud_up.jpg",
    "../assets/textures/skybox/cloudy/bluecloud_dn.jpg",
    "../assets/textures/skybox/cloudy/bluecloud_rt.jpg",
    "../assets/textures/skybox/cloudy/bluecloud_lf.jpg"
};

constexpr std::array<std::string_view, 6> graySkyBox = {
    "../assets/textures/skybox/cloudy/graycloud_ft.jpg",
    "../assets/textures/skybox/cloudy/graycloud_bk.jpg",
    "../assets/textures/skybox/cloudy/graycloud_up.jpg",
    "../assets/textures/skybox/cloudy/graycloud_dn.jpg",
    "../assets/textures/skybox/cloudy/graycloud_rt.jpg",
    "../assets/textures/skybox/cloudy/graycloud_lf.jpg"
};

constexpr std::array<std::string_view, 6> brownSkyBox = {
    "../assets/textures/skybox/cloudy/browncloud_ft.jpg",
    "../assets/textures/skybox/cloudy/browncloud_bk.jpg",
    "../assets/textures/skybox/cloudy/browncloud_up.jpg",
    "../assets/textures/skybox/cloudy/browncloud_dn.jpg",
    "../assets/textures/skybox/cloudy/browncloud_rt.jpg",
    "../assets/textures/skybox/cloudy/browncloud_lf.jpg"
};

constexpr std::array<std::string_view, 6> yellowSkyBox = {
    "../assets/textures/skybox/cloudy/yellowcloud_ft.jpg",
    "../assets/textures/skybox/cloudy/yellowcloud_bk.jpg",
    "../assets/textures/skybox/cloudy/yellowcloud_up.jpg",
    "../assets/textures/skybox/cloudy/yellowcloud_dn.jpg",
    "../assets/textures/skybox/cloudy/yellowcloud_rt.jpg",
    "../assets/textures/skybox/cloudy/yellowcloud_lf.jpg" 
};

const std::string treeSpritePath = "../assets/textures/tree_2_cropped.png";
}
#define SKYBOX_BLUESKY 0
#define SKYBOX_GREYSKY 1
#define SKYBOX_BROWNSKY 2
#define SKYBOX_YELLOWSKY 3

#define BLUESKY_LIGHT_COLOUR glm::vec3{1.0, 1.0, 1.0}
#define GREYSKY_LIGHT_COLOUR glm::vec3{0.631, 0.631 ,0.631}
#define BROWNSKY_LIGHT_COLOUR glm::vec3{0.631, 0.553, 0.396}
#define YELLOWSKY_LIGHT_COLOUR glm::vec3{1.0, 0.725, 0.149}


// Glm::vec3 output definition
inline std::ostream& operator<<(std::ostream& stream, const glm::vec3& vector)
{
    stream << "[" << vector.x << ", " << vector.y << ", " << vector.z << "]";
    return stream;
}

// vectors
template<typename T>
inline std::ostream& operator<<(std::ostream& stream, const std::vector<T> vector)
{
    stream << "[";

    // Edge case
    if (vector.size() == 0)
    {
        stream << "]";
        return stream;
    }

    for (int i = 0; i < vector.size()-1; i++)
    {
        stream << "," << vector[i];
    }
    stream << vector[vector.size()-1] << "]";
    return stream;
} 


/////////////////////////////////////////

// Some basic colour definitions
#define RED   glm::vec3{1, 0, 0}
#define GREEN glm::vec3{0, 1, 0}
#define BLUE  glm::vec3{0, 0, 1}
#define ORANGE glm::vec3{1, 0.5, 0}
#define YELLOW glm::vec3{1, 1, 0}
#define CYAN  glm::vec3{0, 1, 1}
#define PURPLE glm::vec3{0.5, 0, 1}
#define PINK glm::vec3{1, 0, 1}
#define BLACK glm::vec3{0, 0, 0}
#define WHITE glm::vec3{1, 1, 1}

#define DEFAULT_ROAD_COLOUR glm::vec3{0.325490196f, 0.329411765f, 0.309803922f}
#define DEFAULT_BACKGROUND_COLOUR glm::vec3{0.2, 0.3, 0.3}
#define DEFAULT_BB_COLOUR WHITE
//////////// Logging system ////////////


#define LOG(log_type, b) std::cout << "[" << StopWatch::GetTimeElapsed() << " ms \t]" \
                            << log_type << b << std::endl;
#define STRINGIZE2(x) #x
#define STRINGIZE(x) STRINGIZE2(x)

// Custom messages for loggings from sections
#define STATUS_SERV(service) (" [" service "|STATUS ] ")
#define WARN_SERV(service) (" [" service "|WARNING] in " __FILE__ " at line " STRINGIZE(__LINE__)" : ")
#define ERROR_SERV(service) (" [" service "| ERROR ] in " __FILE__ " at line " STRINGIZE(__LINE__)" : ")
#define DEBUG_SERV(service) (" [" service "|#DEBUG#] line: " STRINGIZE(__LINE__)" : ")

#define STATUS " [STATUS ] "
#define WARN (" [WARNING] in " __FILE__ " at line " STRINGIZE(__LINE__)" : ")
#define ERROR (" [ ERROR ] in " __FILE__ " at line " STRINGIZE(__LINE__)" : ")
#define DEBUG (" [=DEBUG=] line: " STRINGIZE(__LINE__)" : ")
///////////////////////////////////////

////////////////// Renderer settings ////////////////////

#define ENABLE_CULL_FACE_MODEL 1                // Back face cull on 3D assets

////////////////////////////////////////////////////////

