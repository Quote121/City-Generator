#pragma once

#include <string>

/*
    Header for global config info such as file paths, constants, data

    Note: Paths are relative to the executable in the bin file
*/

//////////////// SHADERS ////////////////

// Sprite default shaders
const char* sprite_defaultVertShaderPath = "../assets/shaders/default/sprite/sprite_shader.vert";
const char* sprite_defaultFragShaderPath = "../assets/shaders/default/sprite/sprite_shader.frag";

// Object default shaders
const char* object_defaultVertShaderPath = "../assets/shaders/default/object/object_shader.vert";
const char* object_defaultFragShaderPath = "../assets/shaders/default/object/object_shader.frag";

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
