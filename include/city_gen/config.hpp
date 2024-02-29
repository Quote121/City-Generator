#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <vector>

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



// Sun Icon for lights
extern const char* light_icon_texture;

}



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
    for (auto a : vector)
    {
        stream << " " << a;
    }
    stream << "]";
    return stream;
} 


/////////////////////////////////////////

// Some basic colour definitions
#define RED   glm::vec3{1, 0, 0}
#define GREEN glm::vec3{0, 1, 0}
#define BLUE  glm::vec3{0, 0, 1}

#define DEFAULT_ROAD_COLOUR glm::vec3{0.325490196f, 0.329411765f, 0.309803922f}


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
