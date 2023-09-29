#include "menues.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

void Menues::display(Camera* cam)
{
    
    Scene* scene = Scene::getInstance();
    
    
    int i = 0;
    
    ImGui::Begin("Game objects");

    if(ImGui::TreeNode("Objects"))
    {
        for (auto& obj_ptr : scene->getObjects())
        {
            Object* obj = obj_ptr.get();

            // Get the object name from the filepath specifed
            std::stringstream ss_filePath{obj->getFilePath()};
            std::string segment;
            std::vector<std::string> seglist;
            while(std::getline(ss_filePath, segment, '/')) { seglist.push_back(segment); }

            // Give the object a number and its filename
            if (ImGui::TreeNode((void*)(intptr_t)i, "Object %d - %s", i, seglist[seglist.size()-1].c_str()))
            {

                std::stringstream ss_name;
                ss_name << "Controls for " << seglist[seglist.size()-1] << " : ";

                ImGui::Text(ss_name.str().c_str());
                std::stringstream ss_pos;
                ss_pos << "Box " << i << " pos : [X:Y:Z] = " << obj->_position.x << "," << obj->_position.y << "," << obj->_position.z;
                ImGui::Text(ss_pos.str().c_str());
                std::stringstream ss_distance;
                ss_distance << "Box " << i << " is " << glm::length(obj->_position-cam->Position) << " units away from you.";
                ImGui::Text(ss_distance.str().c_str());

                ImGui::NewLine();
                ImGui::Text("Position:\n");
                ImGui::SliderFloat("X pos: ", &obj->_position.x, -100.0f, 100.0f);
                ImGui::SliderFloat("Y pos: ", &obj->_position.y, -100.0f, 100.0f);
                ImGui::SliderFloat("Z pos: ", &obj->_position.z, -100.0f, 100.0f);

                ImGui::Text("Rotation:\n");
                ImGui::SliderFloat("X rotation", &obj->_rotation.x, -10.0f, 100.0f);
                ImGui::SliderFloat("Y rotation", &obj->_rotation.y, -10.0f, 10.0f);
                ImGui::SliderFloat("Z rotation", &obj->_rotation.z, -10.0f, 10.0f);

                ImGui::Text("Scale:\n");
                ImGui::SliderFloat("X scale", &obj->_scale.x, 0.1f, 10.0f);
                ImGui::SliderFloat("Y scale", &obj->_scale.y, 0.1f, 10.0f);
                ImGui::SliderFloat("Z scale", &obj->_scale.z, 0.1f, 10.0f);
                ImGui::SliderFloat("Scale", &obj->_scaleScalar, 0.1f, 10.0f);
                
                ImGui::TreePop();

                if (ImGui::Button("Delete"))
                {
                    // since we have deleted an object we need to re render the imgui list (break)
                    scene->removeObject(*obj);
                    break;
                }
                
            }
            i++;
        }
        ImGui::TreePop();
    }

    // List box of spawnable objects
    static int listbox_currentitem = 0;
    const char* items[] = { "Rat", "Cargo" };
    ImGui::ListBox("Lists:\n", &listbox_currentitem, items, IM_ARRAYSIZE(items));

    if (ImGui::Button("Spawn"))
    {
        ImGui::Text("In development");
    }

    ImGui::End();
}