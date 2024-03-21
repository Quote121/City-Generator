#include "imgui/imgui.h"
#include <menues.hpp>
#include <resourceManager.hpp>
#include <generator.hpp>
#include <string>

#include <camera.hpp>

#define POSITION_MAX 100.0f
#define POSITION_MIN -100.0f

#define ROTATION_MAX 6.28318f // 2PI
#define ROTATION_MIN -6.28318f // -2PI

#define SCALE_MAX 10.0f
#define SCALE_MIN 0.01f


void Menues::display(float deltaTime)
{
    Camera* cam = Camera::getInstance();

    //
    // Window that gives fps, position, camera view
    //
    ImGui::Begin("Stats");
    ImGui::Text("FPS %.2f || Delta time: %.1f ms", 1/deltaTime, deltaTime*1000);
    ImGui::Text("Pitch | Yaw: %.3f %.3f", cam->Pitch, cam->Yaw);
    ImGui::Text("[x,y,z] : %.3f, %.3f, %.3f",cam->Position.x, cam->Position.y, cam->Position.z);
    ImGui::End();
    

    Scene* scene = Scene::getInstance();

    //###############################
    //  Generator controls
    //###############################
    ImGui::Begin("Generator settings");
    // Settings for the road Generator
    ImGui::Text("Road generator settings");

    static long menu_seed = 0;
    ImGui::Text("Seed: %ld", menu_seed);
    
    bool removeRoads = ImGui::Button("Remove all roads.");
    if (removeRoads) { scene->removeAllRoads(); }

    bool generateRoads = ImGui::Button("Clear and Generate.");
    if (generateRoads)
    {
        scene->removeAllModels();
        scene->removeAllRoads();
        scene->removeAllSprites();
        menu_seed = generator::GenerateCity(0);
    }
    
    // For the counts
    ImGui::Text("World count: ");
    ImGui::Text("Objects [%ld]", scene->GetModelObjects().size());
    ImGui::Text("Roads [%ld]", scene->GetRoadObjects().size());
    ImGui::Text("Sprites [%ld]", scene->GetSpriteObjects().size());
    ImGui::Text("Model instance renderers [%ld]", scene->GetModelInstanceRenderers().size());

    static char textBuffer[20] = "";
    bool simulateRandomGen = ImGui::Button("Random generator.");
    if (simulateRandomGen)
    {
        if (std::strcmp(textBuffer, ""))
        {
            LOG(STATUS, "TEXTBUFFER: " << std::stoi(textBuffer));
            menu_seed = generator::GenerateCity(std::stoi(textBuffer));
        }
        else{
            menu_seed = generator::GenerateCity(0);
        }
    }
    ImGui::Text("Seed:");
    ImGui::InputText("##seedInput", textBuffer, 20);



    ImGui::Text("Building generator settings");

    bool removeZoneCollisions = ImGui::Button("Clear zone collisions");
    if (removeZoneCollisions) { generator::ClearZoneCollisions(); }

    bool removeModels = ImGui::Button("Remove all models");
    if (removeModels)
    { scene->removeAllModels(); }

    // bool generateBuildings = ImGui::Button("Generate buildings");
    // if (generateBuildings)
    // {
    //     generator::CalculateValidZones();
    //     generator::GenerateBuildings(1); // TODO remove and only use the values set by the random generator
    // }

    ImGui::End();

    //##############################
    // Selected object menu
    //##############################
    if (scene->sceneSelectedObject->HasObjectSelected())
    {
        ImGui::Begin("Selected object");
        // Remove focus from the window so we dont have to double click to deselect
       
        std::string testText = "";
        bool needsUpdating = false;

        // Model
        switch(scene->sceneSelectedObject->GetType())
        {
            case SceneType::MODEL: {
                ModelObject* object = static_cast<ModelObject*>(scene->sceneSelectedObject->GetObject());

                ImGui::Text("Object scene name: %s", object->GetAlias().c_str());
                ImGui::TextColored(ImVec4{1.0f, 0.2f, 0.2f, 1.0f},"%s is %.3f units away from you.", 
                        object->GetModelName().c_str(), glm::length(object->GetPosition()-cam->Position));

                static glm::vec3 menuPositionBefore = object->GetPosition();
                static glm::vec3 menuPosition = object->GetPosition();

                static glm::vec3 menuRotationBefore = object->GetRotation();
                static glm::vec3 menuRotation = object->GetRotation();

                static glm::vec3 menuScaleBefore = object->GetScale();
                static glm::vec3 menuScale = object->GetScale();
                
                static float menuScalarScaleBefore = object->GetScaleScalar();
                static float menuScalarScale = object->GetScaleScalar();

                ImGui::PushItemWidth(100);
                ImGui::Text("Position:");
                ImGui::SliderFloat("X##POS", &menuPosition.x, POSITION_MIN, POSITION_MAX); ImGui::SameLine();
                ImGui::SliderFloat("Y##POS", &menuPosition.y, POSITION_MIN, POSITION_MAX); ImGui::SameLine();
                ImGui::SliderFloat("Z##POS", &menuPosition.z, POSITION_MIN, POSITION_MAX);
                                        
                ImGui::Text("Rotation:");
                ImGui::SliderFloat("X##ROT", &menuRotation.x, ROTATION_MIN, ROTATION_MAX); ImGui::SameLine();
                ImGui::SliderFloat("Y##ROT", &menuRotation.y, ROTATION_MIN, ROTATION_MAX); ImGui::SameLine();
                ImGui::SliderFloat("Z##ROT", &menuRotation.z, ROTATION_MIN, ROTATION_MAX);
                    
                ImGui::Text("Scale:");
                ImGui::SliderFloat("X##SCL", &menuScale.x, SCALE_MIN, SCALE_MAX); ImGui::SameLine();
                ImGui::SliderFloat("Y##SCL", &menuScale.y, SCALE_MIN, SCALE_MAX); ImGui::SameLine();
                ImGui::SliderFloat("Z##SCL", &menuScale.z, SCALE_MIN, SCALE_MAX); 
                ImGui::SliderFloat("Scale", &menuScalarScale, SCALE_MIN, SCALE_MAX); 
                ImGui::PopItemWidth();

                if (!object->GetIsInstanceRendered())
                {
                    ImGui::Checkbox("Show OBB", &object->GetShowBoundingBoxImGui());
                }

                bool deleteModel = ImGui::Button("Delete");
                if (deleteModel)
                {
                    // Delete and unfocus so the user cant control the object anymore
                    scene->removeModel(*object);
                    scene->sceneSelectedObject->Deselect();
                    // ImGui::SetWindowFocus(nullptr);
                }

                // We check if the values change, if they do we call an update
                if (menuPosition != menuPositionBefore)
                {
                    needsUpdating = true;
                    menuPositionBefore = menuPosition;
                    object->SetPosition(menuPosition);
                }
                if (menuRotation != menuRotationBefore)
                {
                    needsUpdating = true;
                    menuRotationBefore = menuRotation;
                    object->SetRotation(menuRotation);
                }
                if (menuScale != menuScaleBefore)
                {
                    needsUpdating = true;
                    menuScaleBefore = menuScale;
                    object->SetScale(menuScale);
                }
                if (menuScalarScale != menuScalarScaleBefore)
                {
                    needsUpdating = true;
                    menuScalarScaleBefore = menuScalarScale;
                    object->SetScale(menuScalarScale);
                }
                // Call the instance renderer update
                if (needsUpdating && object->GetIsInstanceRendered())
                {
                    scene->GetModelInstanceRenderer(object)->Update(object);
                }
                break;
            }

            case SceneType::ROAD: {
                RoadObject* road = static_cast<RoadObject*>(scene->sceneSelectedObject->GetObject());
                ImGui::TextColored(ImVec4{1.0f, 0.2f, 0.2f, 1.0f},"Scene name: %s.", 
                        road->GetAlias().c_str());

                static glm::vec3 pointA_Before = road->GetPointA();
                static glm::vec3 pointB_Before = road->GetPointB();
                static float widthBefore = road->GetWidth();


                ImGui::PushItemWidth(100);
                ImGui::SliderFloat("Point a.x", &road->GetPointAImGui().x, -200, 200); ImGui::SameLine();
                ImGui::SliderFloat("Point a.z", &road->GetPointAImGui().z, -200, 200);

                ImGui::SliderFloat("Point b.x", &road->GetPointBImGui().x, -200, 200); ImGui::SameLine(); 
                ImGui::SliderFloat("Point b.z", &road->GetPointBImGui().z, -200, 200);

                ImGui::SliderFloat("Width", &road->GetWidthImGui(), 1.0f, 5.0f);
                ImGui::PopItemWidth();

                bool deleteRoad = ImGui::Button("Delete");
                if (deleteRoad)
                {
                    LOG(STATUS, "Functionallity not implemented");
                } 

                if (pointA_Before != road->GetPointA() || pointB_Before != road->GetPointB() || widthBefore != road->GetWidth())
                {
                    // Update before variables and call an update
                    pointA_Before = road->GetPointA();
                    pointB_Before = road->GetPointB();
                    widthBefore = road->GetWidth();
                    road->UpdateRoadAndBatch();
                }
                break;
            }
                
            case SceneType::SPRITE: {

                SpriteObject* sprite = static_cast<SpriteObject*>(scene->sceneSelectedObject->GetObject());
                ImGui::TextColored(ImVec4{1.0f, 0.2f, 0.2f, 1.0f}, "Scene name: %s", sprite->GetAlias().c_str());

                static glm::vec3 position_before = sprite->GetPosition();
                static glm::vec3 rotation_before = sprite->GetRotation();
                static glm::vec2 scale_before = sprite->GetScale();

                ImGui::SliderFloat3("Position", &sprite->GetPositionImGui().x, -200, 200);
                ImGui::SliderFloat3("Rotation", &sprite->GetRotationImGui().x, -M_PI, M_PI);
                ImGui::SliderFloat2("Scale", &sprite->GetScaleImGui().x, 0.1, 10.0);
                ImGui::Checkbox("Billboard", &sprite->GetIsBillboardImGui());

                bool deleteSprite = ImGui::Button("Delete");
                if (deleteSprite)
                {
                    scene->removeSprite(*sprite);
                    scene->sceneSelectedObject->Deselect();
                }

                if (position_before != sprite->GetPosition() || rotation_before != sprite->GetRotation() || scale_before != sprite->GetScale())
                {
                    position_before = sprite->GetPosition();
                    rotation_before = sprite->GetRotation();
                    scale_before = sprite->GetScale();
                }

                break;
            }
            case SceneType::P_LIGHT: {
                break;
            }

            case SceneType::LINE:
            case SceneType::D_LIGHT:
            default:
                LOG(WARN, "Switch default hit.");
        }
        ImGui::End();
    }


    //######################################
    //  World controls
    //######################################
    ImGui::Begin("World controls");

    // Show XYZ lines
    ImGui::Checkbox("Show axis", &scene->GetShowSceneAxisImGui());
    ImGui::Checkbox("Show Skybox", &scene->GetShowSkyBoxImGui());    
    ImGui::Checkbox("Show terrain", &scene->GetShowTerrainImGui());
    
    ImGui::NewLine();

    // For selection
    static int selectedSkyboxIndex = 0;
    static int selectedSkyBoxIndexBefore = 0; // Needed to detect change
    std::vector<const char*> skyboxItems;
    for (auto& a : Scene::getInstance()->GetSkyBoxes())
    {
        skyboxItems.push_back(a->GetAlias().c_str());
    }
    ImGui::Text("Skybox:");
    ImGui::ListBox("## skyBoxList", &selectedSkyboxIndex, skyboxItems.data(), skyboxItems.size(), 4);

    if (selectedSkyboxIndex != selectedSkyBoxIndexBefore)
    {
        scene->SetSkybox(selectedSkyboxIndex);
        selectedSkyBoxIndexBefore = selectedSkyboxIndex;
    }


    ImGui::PushItemWidth(100);
    ImGui::Text("Camera Position:");
    ImGui::SliderFloat("X##POS", &cam->GetPositionHandle().x, POSITION_MIN, POSITION_MAX); ImGui::SameLine();
    ImGui::SliderFloat("Y##POS", &cam->GetPositionHandle().y, POSITION_MIN, POSITION_MAX+600); ImGui::SameLine();
    ImGui::SliderFloat("Z##POS", &cam->GetPositionHandle().z, POSITION_MIN, POSITION_MAX);
    ImGui::PopItemWidth();


    if(ImGui::TreeNode("Spawning"))
    {
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

            ImGui::Text("Spawn models: ");
            if (ImGui::BeginCombo("## combo 1", combo_preview_value))
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
            bool spawn = ImGui::Button("Spawn##model");

            static bool spawnOnMe = false; static bool instanceRender = false;
            ImGui::Checkbox("Spawn at my position", &spawnOnMe);
            ImGui::Checkbox("Instance renderer", &instanceRender); 
            if (spawn)
            {
                ModelObject* addedModel = scene->addModel(models.at(item_current_idx)->GetModelPath(), nullptr, instanceRender);
                if (spawnOnMe)
                {
                    addedModel->SetPosition(Camera::getInstance()->Position);
                }
            }
        }

        {
            // Get loaded models from asset loader
            std::vector<TextureInfo*> textures = ResourceManager::getInstance()->GetLoadedTextures();
            std::vector<std::string> textureNames; // Needed for a local pointer to a string

            for (const auto& texture : textures) {
                textureNames.push_back(texture->fileName);
            }

            const char* items[textures.size()];
            for (size_t i = 0; i < textureNames.size(); ++i) {
                items[i] = textureNames[i].c_str();
            }

            static unsigned int item_current_idx = 0; // Here we store our selection data as an index.
            const char* combo_preview_value = items[item_current_idx];  // Pass in the preview value visible before opening the combo (it could be anything)

            ImGui::Text("Spawn sprites: ");
            if (ImGui::BeginCombo("## combo 2", combo_preview_value))
            {
                for (unsigned int n = 0; n < textures.size(); n++)
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
            bool spawnSprite = ImGui::Button("Spawn##sprite");
            if (spawnSprite)
            {
                LOG(STATUS, "Spawn sprite")
                scene->addSprite(textures.at(item_current_idx)->fileName, nullptr);
                
                // Add other builders here

            }
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
                        ImGui::ColorEdit3("Color", &object->GetLightColourImGui().x); ImGui::PopItemWidth();

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
                        ImGui::ColorEdit3("Color", &object->GetLightColourImGui().x); ImGui::PopItemWidth();

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

        if(ImGui::TreeNode("Roads"))
        {
            auto objects = scene->GetRoadObjects();
            for (unsigned int i = 0; i < objects.size(); i++)
            {
                RoadObject* object = objects[i];
                if (ImGui::TreeNode((void*)(intptr_t)i, "Object %d - %s", i, object->GetAlias().c_str()))
                {
                    ImGui::Text("Road placement text");

                    glm::vec3& a = object->GetPointAImGui();
                    glm::vec3& b = object->GetPointBImGui();

                    ImGui::PushItemWidth(200);

                    ImGui::PushID(i);
                    ImGui::SliderFloat("Point a.x", &a.x, -10, 10); ImGui::SameLine();
                    ImGui::SliderFloat("Point a.z", &a.z, -10, 10);

                    ImGui::SliderFloat("Point b.x", &b.x, -10, 10); ImGui::SameLine(); 
                    ImGui::SliderFloat("Point b.z", &b.z, -10, 10);
                    ImGui::PopID();

                    ImGui::PopItemWidth();

                    bool updatePoints = ImGui::Button("Update points");
                    if (updatePoints) object->UpdatePoints(a, b);

                    ImGui::TreePop();
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
