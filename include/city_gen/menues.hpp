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
#pragma once

// IMGUI
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "scene.hpp"
#include "camera.hpp"

class Menues
{

public:
    // TODO, maybe instead of passing as arg we go off the singleton instance?
    static void display(Camera* cam);
};
