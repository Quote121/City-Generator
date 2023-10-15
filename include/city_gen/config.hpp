#pragma once

#include <string>
#include <glm/glm.hpp>
#include <string>
#include <sstream>

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
}
/////////////////////////////////////////

//////////// Logging system ////////////
#define LOG(log_type, b) std::cout << "[" << __TIME__ << "]" \
                            << log_type << b << std::endl;
#define STRINGIZE2(x) #x
#define STRINGIZE(x) STRINGIZE2(x)
#define STATUS " [STATUS ] "
#define INFO " [STATUS ] "
#define WARN (" [WARNING] in " __FILE__ " at line " STRINGIZE(__LINE__)" : ")
#define ERROR (" [ ERROR ] in " __FILE__ " at line " STRINGIZE(__LINE__)" : ")
///////////////////////////////////////

////////////////// Renderer settings ////////////////////

#define ENABLE_CULL_FACE_MODEL 1

////////////////////////////////////////////////////////

///////////////// Op overloads for datastructures /////////////

// std::ostream& operator<<(std::ostream& os, const glm::vec3& vec)
// {
//     std::stringstream ss;
    
//     // os << float(vec[0]);

//     ss << "[" << vec[0] << ", " << vec[1] << ", " << vec[2] << "]";
//     os << ss.str();
//     return os;
// }

///////////////////////////////////////////////////////////////
