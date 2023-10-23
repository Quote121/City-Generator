#include "menues.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

void Menues::display(float deltaTime)
{
    Camera* cam = Camera::getInstance();
    // Fps and general stats area
    static int fpsSampCount = 0;
    static float fpsSum = 0;
    static float fpsValue;
    // Print fps and coords
    const int sampleSize = 100;
    fpsSum += (1 / deltaTime);
    fpsSampCount += 1;
    if (fpsSampCount == sampleSize) {
        fpsValue = ceil((int)(fpsSum / sampleSize * 10)) / 10;
        fpsSum = 0; fpsSampCount = 0;
    }
    //////////////////////////////////////////////////////////////
    //
    // Window that gives fps, position, camera view
    //
    
    ImGui::Begin("Stats");
    std::stringstream imGuiFPS;
    imGuiFPS << "FPS: " << fpsValue;
    ImGui::Text(imGuiFPS.str().c_str());
    std::stringstream viewss;
    viewss << "Pitch | Yaw: " << cam->Pitch << " " << cam->Yaw;
    ImGui::Text(viewss.str().c_str());
    std::stringstream pos_ss;
    pos_ss << "[x,y,z] : " << cam->GetPositionCoords();
    ImGui::Text(pos_ss.str().c_str());
    ImGui::End();
    //////////////////////////////////////////////////////////////
    
    Scene* scene = Scene::getInstance();
    
    // Later on with the resource manager, the user will be able to select from a list of things to spawn into the world

    int i = 0;

    ImGui::Begin("Game objects");
    if(ImGui::TreeNode("Objects"))
    {
        for (BaseObject* object : scene->getObjects())
        {
            if (ImGui::TreeNode((void*)(intptr_t)i, "Object %d - %s", i, object->GetAlias().c_str()))
            {
                // General base class info
                std::stringstream ss_name;
                ss_name << "Controls for " << object->GetAlias() << " : ";
                ImGui::Text(ss_name.str().c_str());
                std::stringstream ss_pos;
                ss_pos << "Position : [X:Y:Z] = " << object->GetPosition().x << "," << object->GetPosition().y << "," << object->GetPosition().z;
                ImGui::Text(ss_pos.str().c_str());
                std::stringstream ss_distance;
                ss_distance << object->GetAlias() << " is " << glm::length(object->GetPosition()-cam->Position) << " units away from you.";
                ImGui::Text(ss_distance.str().c_str());
                
                ImGui::NewLine();
                ImGui::Text("Position:\n");
                ImGui::SliderFloat("X pos: ", &object->GetPositionImGui().x, -200.0f, 200.0f);
                ImGui::SliderFloat("Y pos: ", &object->GetPositionImGui().y, -200.0f, 200.0f);
                ImGui::SliderFloat("Z pos: ", &object->GetPositionImGui().z, -200.0f, 200.0f);

                ImGui::SliderFloat("X rot: ", &object->GetRotationImGui().x, -5.0f, 5.0f);
                ImGui::SliderFloat("Y rot: ", &object->GetRotationImGui().y, -5.0f, 5.0f);
                ImGui::SliderFloat("Z rot: ", &object->GetRotationImGui().z, -5.0f, 5.0f);

                ImGui::Checkbox("Visible", &object->GetIsVisibleImGui());
                ImGui::Checkbox("Display AABB", &object->GetShowBoundingBoxImGui());


                // Object specific controls
                if (ModelObject* object = dynamic_cast<ModelObject*>(object))
                {

                }
                // Sprite specific controls
                else if (SpriteObject* object = dynamic_cast<SpriteObject*>(object))
                {
                
                }
                // Line specific controls
                else if (LineObject* object = dynamic_cast<LineObject*>(object))
                {

                }
                // Light specific controls
                // else if (LightObject* object = dynamic_cast<LightObject*>(sceneObj))
                // {
                //     LOG(ERROR, "Light object not yet implemented");

                // }
                // Particle specific controls
                // Particle
                // else if (ParticleObject* object = dynamic_cast<ParticleObject*>(obj))
                // {
                //     LOG(ERROR, "Particle object not yet implemented");
                // }
                // Error
                else
                {
                    LOG(ERROR, "Unrecognized object");
                }
                ImGui::TreePop();
            }
            i++;
        }
        ImGui::TreePop();
    }
    ImGui::End();
}
