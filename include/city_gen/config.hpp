#pragma once
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <string>
#include <sstream>

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
/////////////////////////////////////////

//////////// Logging system ////////////
#define LOG(log_type, b) std::cout << "[" << StopWatch::GetTimeElapsed() << " ms]" \
                            << log_type << b << std::endl;
#define STRINGIZE2(x) #x
#define STRINGIZE(x) STRINGIZE2(x)
#define STATUS " [STATUS ] "
#define INFO " [STATUS ] "
#define WARN (" [WARNING] in " __FILE__ " at line " STRINGIZE(__LINE__)" : ")
#define ERROR (" [ ERROR ] in " __FILE__ " at line " STRINGIZE(__LINE__)" : ")
///////////////////////////////////////

////////////////// Renderer settings ////////////////////

#define ENABLE_CULL_FACE_MODEL 1                // Back face cull on 3D assets


////////////////////////////////////////////////////////
