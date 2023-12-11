#include <menues.hpp>
#include <resourceManager.hpp>

#define POSITION_MAX 100.0f
#define POSITION_MIN -100.0f

#define ROTATION_MAX 6.28318f // 2PI
#define ROTATION_MIN -6.28318f // -2PI

#define SCALE_MAX 10.0f
#define SCALE_MIN 0.01f


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
    ImGui::Text("FPS %.2f", fpsValue);
    ImGui::Text("Pitch | Yaw: %.3f %.3f", cam->Pitch, cam->Yaw);
    ImGui::Text("[x,y,z] : %.3f, %.3f, %.3f",cam->Position.x, cam->Position.y, cam->Position.z);
    ImGui::End();
    //////////////////////////////////////////////////////////////
    
    Scene* scene = Scene::getInstance();
    

    // Later on with the resource manager, the user will be able to select from a list of things to spawn into the world


    ImGui::Begin("World");

    // Show XYZ lines
    ImGui::Checkbox("Show axis", &scene->GetShowSceneAxisImGui()); 

    if(ImGui::TreeNode("Spawning"))
    {
        // Get loaded models from asset loader
        std::vector<Model*> models = ResourceManager::getInstance()->GetLoadedModels();
        
        std::vector<std::string> modelNames; // Needed for a local pointer to a string

        for (const auto& model : models) {
            modelNames.push_back(model->GetModelName());
        }

        const char* items[modelNames.size()];
        for (size_t i = 0; i < modelNames.size(); ++i) {
            items[i] = modelNames[i].c_str();
        }

        static unsigned int item_current_idx = 0; // Here we store our selection data as an index.
        const char* combo_preview_value = items[item_current_idx];  // Pass in the preview value visible before opening the combo (it could be anything)
        
        if (ImGui::BeginCombo("combo 1", combo_preview_value))
        {
            for (unsigned int n = 0; n < models.size(); n++)
            {
                const bool is_selected = (item_current_idx == n);
                if (ImGui::Selectable(items[n], is_selected))
                    item_current_idx = n;

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        // If button pressed then we check the item_current_idx and act accordingly
        bool spawn = ImGui::Button("Spawn");
        if (spawn)
        {
            scene->addModel(models.at(item_current_idx)->GetModelPath());
        }

        ImGui::TreePop();
    }



    if(ImGui::TreeNode("Objects"))
    {
        if(ImGui::TreeNode("Lights"))
        {
            if(ImGui::TreeNode("Point Lights"))
            {
                auto objects = scene->GetPointLightObjects();
                for (unsigned int i = 0; i < objects.size(); i++)
                {
                    PointLightObject* object = objects[i];
                    if (ImGui::TreeNode((void*)(intptr_t)i, "Object %d - %s", i, object->GetAlias().c_str()))
                    {
                        ImGui::PushItemWidth(100);
                        ImGui::Text("Position:");
                        ImGui::SliderFloat("X##POS", &object->GetPositionImGui().x, POSITION_MIN, POSITION_MAX); ImGui::SameLine();
                        ImGui::SliderFloat("Y##POS", &object->GetPositionImGui().y, POSITION_MIN, POSITION_MAX); ImGui::SameLine();
                        ImGui::SliderFloat("Z##POS", &object->GetPositionImGui().z, POSITION_MIN, POSITION_MAX);
                        ImGui::PopItemWidth();
                        
                        ImGui::PushItemWidth(300);
                        ImGui::ColorEdit4("Color", &object->GetLightColourImGui().x); ImGui::PopItemWidth();

                        ImGui::PushItemWidth(100);
                        
                        ImGui::Text("Ambient:");
                        ImGui::SliderFloat("X##AMB", &object->GetAmbientImGui().x, 0.0f, 1.0f); ImGui::SameLine();
                        ImGui::SliderFloat("Y##AMB", &object->GetAmbientImGui().y, 0.0f, 1.0f); ImGui::SameLine();
                        ImGui::SliderFloat("Z##AMB", &object->GetAmbientImGui().z, 0.0f, 1.0f);
                        
                        ImGui::Text("Diffuse:");
                        ImGui::SliderFloat("X##DIF", &object->GetDiffuseImGui().x, 0.0f, 1.0f); ImGui::SameLine();
                        ImGui::SliderFloat("Y##DIF", &object->GetDiffuseImGui().y, 0.0f, 1.0f); ImGui::SameLine();
                        ImGui::SliderFloat("Z##DIF", &object->GetDiffuseImGui().z, 0.0f, 1.0f);
                        
                        ImGui::Text("Specular:");
                        ImGui::SliderFloat("X##SPC", &object->GetSpecularImGui().x, 0.0f, 1.0f); ImGui::SameLine();
                        ImGui::SliderFloat("Y##SPC", &object->GetSpecularImGui().y, 0.0f, 1.0f); ImGui::SameLine();
                        ImGui::SliderFloat("Z##SPC", &object->GetSpecularImGui().z, 0.0f, 1.0f);
                        ImGui::PopItemWidth();

                        ImGui::SliderFloat("Constant:", &object->GetConstantImGui(), 0.0f, 1.0f); ImGui::SameLine(); ImGui::NewLine();

                        ImGui::SliderFloat("Linear:", &object->GetLinearImGui(), 0.0f, 1.0f); ImGui::SameLine(); ImGui::NewLine();

                        ImGui::SliderFloat("Quadratic:", &object->GetQuadraticImGui(), 0.0f, 1.0f); ImGui::SameLine(); ImGui::NewLine();
                    
                        ImGui::TreePop();

                        bool deleteBtn = ImGui::Button("Delete");
                        if (deleteBtn)
                        {
                            scene->removePointLight(*object);
                        }
                    }
                }
                ImGui::TreePop();
            }
            if(ImGui::TreeNode("Direction Lights"))
            {
                auto objects = scene->GetDirectionalLightObjects();
                for (unsigned int i = 0; i < objects.size(); i++)
                {
                    DirectionalLightObject* object = objects[i];
                    if (ImGui::TreeNode((void*)(intptr_t)i, "Object %d - %s", i, object->GetAlias().c_str()))
                    {
                        ImGui::PushItemWidth(100);
                        ImGui::Text("Direction:");
                        ImGui::SliderFloat("X##DIR", &object->GetDirectionImGui().x, ROTATION_MIN, ROTATION_MAX); ImGui::SameLine();
                        ImGui::SliderFloat("Y##DIR", &object->GetDirectionImGui().y, ROTATION_MIN, ROTATION_MAX); ImGui::SameLine();
                        ImGui::SliderFloat("Z##DIR", &object->GetDirectionImGui().z, ROTATION_MIN, ROTATION_MAX);
                        ImGui::PopItemWidth();

                        ImGui::PushItemWidth(300);
                        ImGui::ColorEdit4("Color", &object->GetLightColourImGui().x); ImGui::PopItemWidth();

                        ImGui::PushItemWidth(100);
                        ImGui::Text("Ambient:");
                        ImGui::SliderFloat("X##AMB", &object->GetAmbientImGui().x, 0.0f, 1.0f); ImGui::SameLine();
                        ImGui::SliderFloat("Y##AMB", &object->GetAmbientImGui().y, 0.0f, 1.0f); ImGui::SameLine();
                        ImGui::SliderFloat("Z##AMB", &object->GetAmbientImGui().z, 0.0f, 1.0f);
                        
                        ImGui::Text("Diffuse:");
                        ImGui::SliderFloat("X##DIF", &object->GetDiffuseImGui().x, 0.0f, 1.0f); ImGui::SameLine();
                        ImGui::SliderFloat("Y##DIF", &object->GetDiffuseImGui().y, 0.0f, 1.0f); ImGui::SameLine();
                        ImGui::SliderFloat("Z##DIF", &object->GetDiffuseImGui().z, 0.0f, 1.0f);
                        
                        ImGui::Text("Specular:");
                        ImGui::SliderFloat("X##SPC", &object->GetSpecularImGui().x, 0.0f, 1.0f); ImGui::SameLine();
                        ImGui::SliderFloat("Y##SPC", &object->GetSpecularImGui().y, 0.0f, 1.0f); ImGui::SameLine();
                        ImGui::SliderFloat("Z##SPC", &object->GetSpecularImGui().z, 0.0f, 1.0f);
                        ImGui::PopItemWidth();

                        ImGui::TreePop();

                    }
                }
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
        // 3D objects
        if(ImGui::TreeNode("objects"))
        {
            auto objects = scene->GetModelObjects();
            for (unsigned int i = 0; i < objects.size(); i++)
            {
                ModelObject* object = objects[i];
                if (ImGui::TreeNode((void*)(intptr_t)i, "Object %d - %s", i, object->GetAlias().c_str()))
                {
                    ImGui::TextColored(ImVec4{1.0f, 0.2f, 0.2f, 1.0f},"%s is %.3f units away from you.", object->GetModelName().c_str(), glm::length(object->GetPosition()-cam->Position));

                    ImGui::PushItemWidth(100);
                    ImGui::Text("Position:");
                    ImGui::SliderFloat("X##POS", &object->GetPositionImGui().x, POSITION_MIN, POSITION_MAX); ImGui::SameLine();
                    ImGui::SliderFloat("Y##POS", &object->GetPositionImGui().y, POSITION_MIN, POSITION_MAX); ImGui::SameLine();
                    ImGui::SliderFloat("Z##POS", &object->GetPositionImGui().z, POSITION_MIN, POSITION_MAX);
                                        
                    ImGui::Text("Rotation:");
                    ImGui::SliderFloat("X##ROT", &object->GetRotationImGui().x, ROTATION_MIN, ROTATION_MAX); ImGui::SameLine();
                    ImGui::SliderFloat("Y##ROT", &object->GetRotationImGui().y, ROTATION_MIN, ROTATION_MAX); ImGui::SameLine();
                    ImGui::SliderFloat("Z##ROT", &object->GetRotationImGui().z, ROTATION_MIN, ROTATION_MAX);
                    
                    ImGui::Text("Scale:");
                    ImGui::SliderFloat("X##SCL", &object->GetScaleImGui().x, SCALE_MIN, SCALE_MAX); ImGui::SameLine();
                    ImGui::SliderFloat("Y##SCL", &object->GetScaleImGui().y, SCALE_MIN, SCALE_MAX); ImGui::SameLine();
                    ImGui::SliderFloat("Z##SCL", &object->GetScaleImGui().z, SCALE_MIN, SCALE_MAX); 
                    ImGui::SliderFloat("Scale", &object->GetScaleScalarImGui(), SCALE_MIN, SCALE_MAX); 
                    ImGui::PopItemWidth();

                    ImGui::Checkbox("Display AA-BB", &object->GetShowBoundingBoxImGui()); 
                    ImGui::Checkbox("Display ", &object->GetIsVisibleImGui()); 
                    ImGui::Checkbox("Lighting", &object->GetShowLightingImGui());

                    ImGui::TreePop();

                    bool deleteBtn = ImGui::Button("Delete");
                    if (deleteBtn)
                    {
                        scene->removeModel(*object);
                    }
                }
            }
            ImGui::TreePop();
        }
        // 2D sprites
        if(ImGui::TreeNode("Sprites"))
        {
            auto objects = scene->GetSpriteObjects();
            for (unsigned int i = 0; i < objects.size(); i++)
            {
                SpriteObject* object = objects[i];
                if (ImGui::TreeNode((void*)(intptr_t)i, "Object %d - %s", i, object->GetAlias().c_str()))
                {
                    ImGui::TextColored(ImVec4{1.0f, 0.2f, 0.2f, 1.0f},"%s is %.3f units away from you.", object->GetSpriteName().c_str(), glm::length(object->GetPosition()-cam->Position));

                    ImGui::PushItemWidth(100);
                    ImGui::Text("Position:");
                    ImGui::SliderFloat("X##POS", &object->GetPositionImGui().x, POSITION_MIN, POSITION_MAX); ImGui::SameLine();
                    ImGui::SliderFloat("Y##POS", &object->GetPositionImGui().y, POSITION_MIN, POSITION_MAX); ImGui::SameLine();
                    ImGui::SliderFloat("Z##POS", &object->GetPositionImGui().z, POSITION_MIN, POSITION_MAX);
                                        
                    ImGui::Text("Rotation:");
                    ImGui::SliderFloat("X##ROT", &object->GetRotationImGui().x, ROTATION_MIN, ROTATION_MAX); ImGui::SameLine();
                    ImGui::SliderFloat("Y##ROT", &object->GetRotationImGui().y, ROTATION_MIN, ROTATION_MAX); ImGui::SameLine();
                    ImGui::SliderFloat("Z##ROT", &object->GetRotationImGui().z, ROTATION_MIN, ROTATION_MAX);
                    
                    ImGui::Text("Scale:");
                    ImGui::SliderFloat("X##SCL", &object->GetScaleImGui().x, SCALE_MIN, SCALE_MAX); ImGui::SameLine();
                    ImGui::SliderFloat("Y##SCL", &object->GetScaleImGui().y, SCALE_MIN, SCALE_MAX); 
                    ImGui::SliderFloat("Scale", &object->GetScaleScalarImGui(), SCALE_MIN, SCALE_MAX);
                    ImGui::PopItemWidth();

                    ImGui::Checkbox("Toggle billboard", &object->GetIsBillboardImGui());
                    ImGui::Checkbox("Display ", &object->GetIsVisibleImGui());

                    ImGui::TreePop();
                    
                    bool deleteBtn = ImGui::Button("Delete");
                    if (deleteBtn)
                    {
                        scene->removeSprite(*object);
                    }
                }
            }
            ImGui::TreePop();
        }
        // Lines
        if(ImGui::TreeNode("Lines"))
        {
            auto objects = scene->GetLineObjects();
            for (unsigned int i = 0; i < objects.size(); i++)
            {
                LineObject* object = objects[i];
                if (ImGui::TreeNode((void*)(intptr_t)i, "Object %d - %s", i, object->GetAlias().c_str()))
                {
                    ImGui::PushItemWidth(100);
                    ImGui::Text("Point A:");
                    ImGui::SliderFloat("X##A", &object->GetPointAImGui().x, -100.0f, 100.0f); ImGui::SameLine();
                    ImGui::SliderFloat("Y##A", &object->GetPointAImGui().y, -100.0f, 100.0f); ImGui::SameLine();
                    ImGui::SliderFloat("Z##A", &object->GetPointAImGui().z, -100.0f, 100.0f);

                    ImGui::Text("Point B:\n");
                    ImGui::SliderFloat("X##B", &object->GetPointBImGui().x, -100.0f, 100.0f); ImGui::SameLine();
                    ImGui::SliderFloat("Y##B", &object->GetPointBImGui().y, -100.0f, 100.0f); ImGui::SameLine();
                    ImGui::SliderFloat("Z##B", &object->GetPointBImGui().z, -100.0f, 100.0f);
                    ImGui::PopItemWidth();

                    ImGui::PushItemWidth(300);
                    ImGui::ColorEdit4("Color", &object->GetColourImGui().x); ImGui::PopItemWidth();

                    ImGui::TreePop();

                    bool deleteBtn = ImGui::Button("Delete");
                    if (deleteBtn)
                    {
                        scene->removeLine(*object);
                    }
                }
            }
            ImGui::TreePop();
        }
        ImGui::TreePop();
    }
    ImGui::End();
}
