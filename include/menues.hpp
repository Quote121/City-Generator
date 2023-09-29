//////////////
//
// This class is to encapsulate all imgui code to keep it seperate from the main file
//
// For start we will write a static funtion that main can call which will display all ImGui windows
// Within the imgui windows I want to first setup somthing that allows me to see all objects in a scene
// and manipulate certain variables about it:
//  Colour
//  Position
//  Roation
//  Speed
//  Velocity?
//  Scale
// To name a few
//
// this will require the scene class as this class will allow me to access each object within a scene
// this needs to be put on a new branch 
//
///////////////

#ifndef MENUES_HPP
#define MENUES_HPP

// IMGUI
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "scene.hpp"
#include "object.hpp"
#include "camera.hpp"

class Menues
{

public:
    static void display(Camera* cam);
};

#endif // MENUES_HPP