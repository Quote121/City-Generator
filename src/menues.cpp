#include "menues.hpp"



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
        std::vector<BaseObject*> objects;
        objects.insert(objects.end(), scene->GetLineObjects().begin(), scene->GetLineObjects().end());
        objects.insert(objects.end(), scene->GetModelObjects().begin(), scene->GetModelObjects().end());
        objects.insert(objects.end(), scene->GetSpriteObjects().begin(), scene->GetSpriteObjects().end());

        for (BaseObject* object : objects)
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
                


                // Object specific controls
                if (ModelObject* modelObject = dynamic_cast<ModelObject*>(object))
                {
                    std::string nameString = "\nModel : " + modelObject->GetModelName();
                    // ImGui::Text(nameString.c_str());
                    ImGui::TextColored(ImVec4{1.0f, 0.0f, 0.0f, 1.0f}, nameString.c_str());

                    ImGui::Text("Scale:\n");
                    ImGui::SliderFloat("X scale", &modelObject->GetScaleImGui().x, 0.1f, 10.0f);
                    ImGui::SliderFloat("Y scale", &modelObject->GetScaleImGui().y, 0.1f, 10.0f);
                    ImGui::SliderFloat("Z scale", &modelObject->GetScaleImGui().z, 0.1f, 10.0f);
                    ImGui::SliderFloat("Scale", &modelObject->GetScaleScalarImGui(), 0.1f, 10.0f);

                }
                // Sprite specific controls
                else if (SpriteObject* spriteObject = dynamic_cast<SpriteObject*>(object))
                {
                    std::string nameString = "\nSprite : " + spriteObject->GetSpriteName();
                    ImGui::TextColored(ImVec4{1.0f, 0.0f, 0.0f, 1.0f}, nameString.c_str());

                    ImGui::Text("Scale:\n");
                    ImGui::SliderFloat("X scale", &spriteObject->GetScaleImGui().x, 0.1f, 10.0f);
                    ImGui::SliderFloat("Y scale", &spriteObject->GetScaleImGui().y, 0.1f, 10.0f);
                    ImGui::SliderFloat("Scale", &spriteObject->GetScaleScalarImGui(), 0.1f, 10.0f);

                    ImGui::Checkbox("Toggle billboard", &spriteObject->GetIsBillboardImGui());

                }
                // Line specific controls
                else if (LineObject* lineObject = dynamic_cast<LineObject*>(object))
                {

                    ImGui::Text("Scale:\n");
                    ImGui::SliderFloat("X scale", &lineObject->GetScaleImGui().x, 0.1f, 10.0f);
                    ImGui::SliderFloat("Y scale", &lineObject->GetScaleImGui().y, 0.1f, 10.0f);
                    ImGui::SliderFloat("Z scale", &lineObject->GetScaleImGui().z, 0.1f, 10.0f);
                    ImGui::SliderFloat("Scale", &lineObject->GetScaleScalarImGui(), 0.1f, 10.0f);

                    ImGui::Text("Point A:\n");
                    ImGui::SliderFloat("X", &lineObject->GetPointAImGui().x, -100.0f, 100.0f);
                    ImGui::SliderFloat("Y", &lineObject->GetPointAImGui().y, -100.0f, 100.0f);
                    ImGui::SliderFloat("Z", &lineObject->GetPointAImGui().z, -100.0f, 100.0f);

                    ImGui::Text("Point B:\n");
                    ImGui::SliderFloat("X", &lineObject->GetPointBImGui().x, -100.0f, 100.0f);
                    ImGui::SliderFloat("Y", &lineObject->GetPointBImGui().y, -100.0f, 100.0f);
                    ImGui::SliderFloat("Z", &lineObject->GetPointBImGui().z, -100.0f, 100.0f);

                    ImGui::Text("Colour:\n");
                    ImGui::SliderFloat("R", &lineObject->GetColourImGui().x, 0.0f, 1.0f);
                    ImGui::SliderFloat("G", &lineObject->GetColourImGui().y, 0.0f, 1.0f);
                    ImGui::SliderFloat("B", &lineObject->GetColourImGui().z, 0.0f, 1.0f);
                }
                // Light specific controls
                // else if (LightObject* object = dynamic_cast<LightObject*>(sceneObj))
                // {
                //     LOG(ERROR, "Light object not yet implemented");

                // }
                // Particle specific controls
                else if (ParticleObject* particleObject = dynamic_cast<ParticleObject*>(object))
                {
                    LOG(ERROR, "Particle object not yet implemented");
                }
                // Error cant cast the object
                else
                {
                    LOG(ERROR, "Unrecognized object: " << i << " name : ");
                }


                ImGui::NewLine();
                ImGui::Text("Position:\n");
                ImGui::SliderFloat("X pos: ", &object->GetPositionImGui().x, -200.0f, 200.0f);
                ImGui::SliderFloat("Y pos: ", &object->GetPositionImGui().y, -200.0f, 200.0f);
                ImGui::SliderFloat("Z pos: ", &object->GetPositionImGui().z, -200.0f, 200.0f);
                ImGui::NewLine();
                ImGui::Text("Rotation:\n");
                ImGui::SliderFloat("X rot: ", &object->GetRotationImGui().x, -5.0f, 5.0f);
                ImGui::SliderFloat("Y rot: ", &object->GetRotationImGui().y, -5.0f, 5.0f);
                ImGui::SliderFloat("Z rot: ", &object->GetRotationImGui().z, -5.0f, 5.0f);

                ImGui::Checkbox("Visible", &object->GetIsVisibleImGui());
                ImGui::Checkbox("Display AABB", &object->GetShowBoundingBoxImGui());







                ImGui::TreePop();
            }
            i++;
        }
        ImGui::TreePop();
    }
    ImGui::End();
}
