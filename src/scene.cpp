#include "lights/directionalLight_object.hpp"
#include "road_object.hpp"
#include <scene.hpp>
#include <resourceManager.hpp>
#include <algorithm>

Scene* Scene::pInstance{nullptr};

Scene* Scene::getInstance()
{
    if (pInstance == nullptr)
    {
        pInstance = new Scene();
    }
    return pInstance; 
}

Scene::Scene()
{
    // X Y Z (R G B) Lines for the orientation
    // X is Red
    this->addLineAxis(glm::vec3{-1000.0, 0, 0}, glm::vec3{1000.0, 0, 0})
        ->SetColour(glm::vec3{1, 0, 0});
    // Y is Green
    this->addLineAxis(glm::vec3{0, -1000.0, 0}, glm::vec3{0, 1000.0, 0})
        ->SetColour(glm::vec3{0, 1, 0});
    // Z is Blue
    this->addLineAxis(glm::vec3{0, 0, -1000.0}, glm::vec3{0, 0, 1000.0})
        ->SetColour(glm::vec3{0, 0, 1});

    roadBatchRenderer = new BatchRenderer(); 
}


// Deallocate all created objects
Scene::~Scene()
{
    delete(roadBatchRenderer);
    delete(pInstance);

    // Free all allocated objects
    removeAllModels();
    removeAllSprites();
    removeAllLines();
    removeAllPointLights();
    removeAllDirectionalLights();
    removeAllRoads();
}


ModelObject* Scene::addModel(const std::string& modelPath_in,
                      const ShaderPath* shader_in)
{
    // If we have passed nullptr load default shader
    Shader* shader = nullptr;
    if (shader_in != nullptr)
    {
        shader = ResourceManager::getInstance()->LoadShader(shader_in->vertex, shader_in->fragment);
    }

    ModelObject* model = new ModelObject(
        modelPath_in, shader
    );
    scene_model_objects.push_back(model);

    modelCount++;

    std::string name = "Model_" + std::to_string(modelCount);
    model->SetAlias(&name);
    
    return model;
}

// Roads
RoadObject* Scene::addRoad(glm::vec3 point_a,
                           glm::vec3 point_b,
                           float road_width,
                           const ShaderPath* shader_in)
{
    Shader* shader = nullptr;
    if (shader_in != nullptr)
    {
        shader = ResourceManager::getInstance()->LoadShader(shader_in->vertex, shader_in->fragment);
    }

    // Make sure we cant have a negative or too small road width
    if (road_width < 0.1)
    {
        road_width = 0.1;
    }

    RoadObject* road = new RoadObject(
        point_a, point_b, road_width, shader
    );

    scene_road_objects.push_back(road);
    roadCount++;

    std::string name = "Road_" + std::to_string(roadCount);
    road->SetAlias(name);

    return road;
}



SpriteObject* Scene::addSprite(std::string& spriteTexture_in,
                               const ShaderPath* shader_in)
{
    // If we have passed nullptr load default shader
    Shader* shader = nullptr;
    if (shader_in != nullptr)
    {
        shader = ResourceManager::getInstance()->LoadShader(shader_in->vertex, shader_in->fragment);
    }

    // Create sprite, allocate memory and put in list
    SpriteObject* sprite = new SpriteObject(
        spriteTexture_in, shader
    );
    scene_sprite_objects.push_back(sprite);
    
    spriteCount++;

    std::string name = "Sprite_" + std::to_string(spriteCount);
    sprite->SetAlias(&name);
    
    return sprite;
}


LineObject* Scene::addLine(glm::vec3 point_a,
                           glm::vec3 point_b,
                           const ShaderPath* shader_in)
{
    // If we have passed nullptr load default shader
    Shader* shader = nullptr;
    if (shader_in != nullptr)
    {
        shader = ResourceManager::getInstance()->LoadShader(shader_in->vertex, shader_in->fragment);
    }

    // Create line
    LineObject* line = new LineObject(
        shader, point_a, point_b
    );
    scene_line_objects.push_back(line);

    lineCount++;

    std::string name = "Line_" + std::to_string(lineCount);
    line->SetAlias(&name);

    return line;
}

LineObject* Scene::addLineAxis(glm::vec3 point_a,
                           glm::vec3 point_b,
                           const ShaderPath* shader_in)
{
    // If we have passed nullptr load default shader
    Shader* shader = nullptr;
    if (shader_in != nullptr)
    {
        shader = ResourceManager::getInstance()->LoadShader(shader_in->vertex, shader_in->fragment);
    }

    // Create line
    LineObject* line = new LineObject(
        shader, point_a, point_b
    );
    scene_axis_lines.push_back(line);

    return line;
}


DirectionalLightObject* Scene::addDirectionalLight()
{
    DirectionalLightObject* light = new DirectionalLightObject();

    scene_directionalLight_objects.push_back(light);

    dirLightCount++;

    std::string lightString = "Light_dir_" + std::to_string(dirLightCount);
    light->SetAlias(&lightString);

    return light;
}

PointLightObject* Scene::addPointLight()
{
    PointLightObject* light = new PointLightObject();

    scene_pointLight_objects.push_back(light);

    pointLightCount++;

    std::string lightString = "Light_pnt_" + std::to_string(pointLightCount);
    light->SetAlias(&lightString);
    

    return light;
}


// Remove objects
void Scene::removeModel(ModelObject& obj)
{
    auto it = std::find(scene_model_objects.begin(), scene_model_objects.end(), &obj);
    if (it != scene_model_objects.end())
    {
        scene_model_objects.erase(it);
    }
}

void Scene::removeSprite(SpriteObject& obj)
{
    auto it = std::find(scene_sprite_objects.begin(), scene_sprite_objects.end(), &obj);
    if (it != scene_sprite_objects.end())
    {
        scene_sprite_objects.erase(it);
    }    
}

void Scene::removeLine(LineObject& obj)
{
    auto it = std::find(scene_line_objects.begin(), scene_line_objects.end(), &obj);
    if (it != scene_line_objects.end())
    {
        scene_line_objects.erase(it);
    }
}

void Scene::removePointLight(PointLightObject& obj)
{
    auto it = std::find(scene_pointLight_objects.begin(), scene_pointLight_objects.end(), &obj);
    if (it != scene_pointLight_objects.end())
    {
        scene_pointLight_objects.erase(it);
    }
}

void Scene::removeDirectionalLight(DirectionalLightObject& obj)
{
    auto it = std::find(scene_directionalLight_objects.begin(), scene_directionalLight_objects.end(), &obj);
    if (it != scene_directionalLight_objects.end())
    {
        scene_directionalLight_objects.erase(it);
    }
}

void Scene::removeRoad(RoadObject& obj)
{
    auto it = std::find(scene_road_objects.begin(), scene_road_objects.end(), &obj);
    if (it != scene_road_objects.end())
    {
        scene_road_objects.erase(it);
    }
}


// Method implementations for removing all objects from each vector
// None of the removeAll.. methods are thread safe
void Scene::removeAllModels(void)
{
    // Call destructor for each object
    for (auto& obj : scene_model_objects)
    {
        delete(obj);
    }
    // Then empty the vector
    scene_model_objects.clear();
}

void Scene::removeAllSprites(void)
{
    for (auto& obj : scene_sprite_objects)
    {
        delete(obj);
    }
    scene_sprite_objects.clear();
}

void Scene::removeAllLines(void)
{
    for (auto& obj : scene_line_objects)
    {
        delete(obj);
    }
    scene_line_objects.clear();
}

void Scene::removeAllPointLights(void)
{
    for (auto& obj : scene_pointLight_objects)
    {
        delete(obj);
    }
    scene_pointLight_objects.clear();
}

void Scene::removeAllDirectionalLights(void)
{
    for (auto& obj : scene_directionalLight_objects)
    {
        delete(obj);
    }
    scene_directionalLight_objects.clear();
}

void Scene::removeAllRoads(void)
{
    for (auto& obj : scene_road_objects)
    {
        delete(obj);
    }
    scene_road_objects.clear();
}


// BaseObject is a class template and so we have to specify the objects we are comparing
template<class T, class U>
bool Scene::SortByDistanceInv(BaseObject<T>* a, BaseObject<U>* b)
{   
    return a->GetDistanceFromCamera() > b->GetDistanceFromCamera();
}

void Scene::DrawSceneObjects(glm::mat4 view, glm::mat4 projection)
{
    // Draw lines
    for (auto& line : GetLineObjects())
    {
        line->Draw(view, projection);
    }

    // Axis
    if (showSceneAxis)
    {
        for (auto& line : scene_axis_lines)
        {
            line->Draw(view, projection);
        }
    }
   
    // Instance model drawing:
    //
    // For each model, if not in vector add and then get all the same momdels
    // Do until end of model objects and then call each of their instance renderers


    // // NOTE
    // 
    // For all model drawn if they are to be instanced, the only difference
    // is their model matrix. If we want the same model to have other qualities
    // then we need to render it normally. For this we should have a bool to indicate
    // if it should be instance rendered or normally rendered.
    //
    
    std::vector<std::string> instancedModels;

    // Draw objects
    for (auto& obj : GetModelObjects())
    {
        std::vector<float> matrices;
        
        if (obj->GetIsInstanceRendered()) 
        {
            if (std::find(instancedModels.begin(), instancedModels.end(), obj->GetModelName()) != instancedModels.end())
            {
                // Skip this object as we've already done it
                continue;
            }

            // Check if we've already instanced rendered it, if not then we get all the 
            // same models and instance render
            for (auto& obj2 : GetModelObjects())
            {
                if (obj2->GetModelName() == obj->GetModelName())
                {
                    glm::mat4 matrix = obj2->GetModelMatrix();

                    for (int i = 0; i < 4; i++)
                        matrices.insert(matrices.end(), {matrix[i].x, matrix[i].y, matrix[i].z, matrix[i].w});
                }
            }
            instancedModels.push_back(obj->GetModelName());
            obj->DrawInstances(view, projection, &matrices);
        }
        // If not instanced, draw normally
        else 
        {
            obj->Draw(view, projection);
        }        
        

        // glm::mat4 matrix = obj->GetModelMatrix();
        // for (int i = 0; i < 4; i++)
        // {
        //     matrices.insert(matrices.end(), {matrix[i].x, matrix[i].y, matrix[i].z, matrix[i].w});
        // }
        // obj->Draw(view, projection);
        
        // For all the same model names
        // Append them to the temp matrices vector and then call the DrawInstances

        // obj->GetModelName();
    }
    // Assume all are the same TEST
    // GetModelObjects()[0]->DrawInstances(view, projection, &matrices);
     
    // Scene::getInstance()-> 

    roadBatchRenderer->DrawBatch(view, projection);

    // Draw roadsscene
    // for (auto& obj : GetRoadObjects())
    // {
    //   obj->Draw(view, projection);
    // }


    // Get the sprites and point light sprites and order both by distance to 
    // avoid alpha bug, this inner vector also avoids bug of ImGui menu displaying
    // sprites in order of distance from camera
    std::vector<SpriteObject*> sprites;
    sprites.insert(sprites.end(), scene_sprite_objects.begin(), scene_sprite_objects.end());
    sprites.insert(sprites.end(), 
            scene_pointLight_objects.begin(), 
            scene_pointLight_objects.end());

    std::sort(sprites.begin(), sprites.end(), SortByDistanceInv<SpriteObject, SpriteObject>);

    for (auto& sprite : sprites)
    {
        sprite->Draw(view, projection);
    }
}


void Scene::CreateSkyBox(std::vector<std::string>* images)
{
    // In here we create the skybox object and allocate it memory.
    // This will make a call to skybox.cpp and skybox.hpp which keeps the opengl 
    // code in renderer
    //
    // Scene will have a renderer which can then be used
    skybox = new SkyBox(*images);
}

void Scene::DrawSkyBox(glm::mat4 view, glm::mat4 projection)
{
    // We call the skybox draw call from the skybox object we created.
    if (showSkybox)
    {
        skybox->Draw(view, projection);
    }
}
