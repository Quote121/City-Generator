#include <scene.hpp>
#include <lights/directionalLight_object.hpp>
#include <road_object.hpp>
#include <resourceManager.hpp>
#include <algorithm>
#include <camera.hpp>

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
    sceneSelectedObject = new SelectedObject();
}


// Deallocate all created objects
Scene::~Scene()
{
    delete(roadBatchRenderer);
    delete(sceneSelectedObject);

    // Free all allocated objects
    removeAllModels();
    removeAllSprites();
    removeAllLines();
    removeAllPointLights();
    removeAllDirectionalLights();
    removeAllRoads();
    removeAllInstanceRenderers();

    delete(pInstance);
}

ModelObject* Scene::addTerrain(const std::string& modelPath_in,
                               const ShaderPath* shader_in)
{
    Shader* shader = nullptr;
    if (shader_in != nullptr)
        shader = ResourceManager::getInstance()->LoadShader(shader_in->vertex, shader_in->fragment);

    ModelObject* model = new ModelObject(
            modelPath_in, shader
    );
    this->terrain = model;
    return model;
}


void Scene::addModelToInstanceRenderer(ModelObject* modelObject_in,
                                       const std::string& modelPath_in,
                                       const ShaderPath* shader_in)
{
    bool added = false;
    // We check all of the instance renderers for a same object
    for (auto& ir : modelInstanceRenderers)
    {
        // Same model must be added
        if (ir->GetInstanceType()->GetModelPath() == modelPath_in)
        {
            added = true;
            ir->Append(modelObject_in);
        }
    }
    if (!added)
    {
        InstanceRenderer<ModelObject*>* IR = new InstanceRenderer<ModelObject*>(); 
        IR->Append(modelObject_in);
        modelInstanceRenderers.push_back(IR);
    }
}



void Scene::ForceReloadInstanceRendererData(void) const
{
    for (auto& a : modelInstanceRenderers)
    {
        a->UpdateAll();
    }
}


ModelObject* Scene::addModel(const std::string& modelPath_in,
                             const ShaderPath* shader_in,
                             const bool instanced)
{
    // If we have passed nullptr load default shader
    Shader* shader = nullptr;
    if (shader_in != nullptr)
    {
        shader = ResourceManager::getInstance()->LoadShader(shader_in->vertex, shader_in->fragment);
    }
    else {
        shader = ResourceManager::getInstance()->
            LoadShader(paths::object_defaultVertShaderPath, paths::object_defaultFragShaderPath);
    }

    ModelObject* model = new ModelObject(
        modelPath_in, shader
    );
    
    // Set instanced boolean and add to an instanceRenderer
    model->SetInstaceRendering(instanced);
    if (instanced)
    {
        addModelToInstanceRenderer(model, modelPath_in, shader_in);
    }

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
                               const ShaderPath* shader_in,
                               const bool instanced)
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
                           const ShaderPath* shader_in,
                           const bool instanced)
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
    // remove all models from the instance renderer
    for(auto& ir : modelInstanceRenderers)
    {
        ir->Clear();
        this->removeAllInstanceRenderers();
    }

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

void Scene::removeAllInstanceRenderers(void)
{
    for (auto& a : modelInstanceRenderers)
    {
        delete(a);
    }
    modelInstanceRenderers.clear();
    
    // for (auto& a : spriteInstanceRenderers)
    // {
    //     delete(a);
    // }
    // spriteInstanceRenderers.clear();
    //
    // for(auto& a : lineInstanceRenderers)
    // {
    //     delete(a);
    // }
    // lineInstanceRenderers.clear();
}


// BaseObject is a class template and so we have to specify the objects we are comparing
template<class T, class U>
bool Scene::SortByDistanceInv(BaseObject<T>* a, BaseObject<U>* b)
{   
    return a->GetDistanceFromCamera() > b->GetDistanceFromCamera();
}

void Scene::DrawSceneObjects()
{
    glm::mat4 view = Camera::getInstance()->GetViewMatrix();
    glm::mat4 projection = Camera::getInstance()->GetProjectionMatrix();

    // Draw terrain
    if (showTerrain)
    {
        this->terrain->Draw(view, projection);
    }

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

    // Draw all instanceRenderers
    //
    auto modelInstanceStartTime = StopWatch::GetCurrentTimePoint();
    for (auto& renderer : modelInstanceRenderers)
    {
        renderer->Draw();
    }

    for (auto& object : scene_model_objects)
    {
        if (!object->GetIsInstanceRendered())
        {
            object->Draw(view, projection);
        }
    }

    uint64_t intsanceDrawTimeElapsed = StopWatch::GetTimeElapsed(modelInstanceStartTime);
    LOG(STATUS, "INSTANCE DRAW TIME: " << intsanceDrawTimeElapsed << "ms")
    
    // Draw all the roads
    roadBatchRenderer->DrawBatch(view, projection);

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

void Scene::DrawSkyBox()
{
    glm::mat4 projection = Camera::getInstance()->GetProjectionMatrix();
    // Remove translation from matrix by casting to mat3 then mat4
    glm::mat4 view = glm::mat4(glm::mat3(Camera::getInstance()->GetViewMatrix()));

    // We call the skybox draw call from the skybox object we created.
    if (showSkybox)
    {
        skybox->Draw(view, projection);
    }
}

// Credit : used from : https://github.com/opengl-tutorials/ogl/tree/master
// Intersection calculation function
bool TestRayOBBIntersection(
    glm::vec3 rayOrigin,
    glm::vec3 rayDirection,
    glm::vec3 boundingBoxMin,
    glm::vec3 boundingBoxMax,
    glm::mat4 modelMatrix,
    float &distanceToHit
){
    float tMin = 0.0f;
    float tMax = 100000.0f;

    glm::vec3 OBBposition_worldspace(modelMatrix[3].x, modelMatrix[3].y, modelMatrix[3].z);
    glm::vec3 delta = OBBposition_worldspace - rayOrigin;

    // Using scope to make sure e,f,t1,t2 can be the same for each plane test
    {
        // We test each axis:
        // X - axis
        glm::vec3 xaxis(modelMatrix[0].x, modelMatrix[0].y, modelMatrix[0].z);
        float e = glm::dot(xaxis, delta);
        float f = glm::dot(rayDirection, xaxis);

        // If f is near 0 do not do division
        if (fabs(f) > 0.0001f)
        {
            float t1 = (e+boundingBoxMin.x)/f;
            float t2 = (e+boundingBoxMax.x)/f;

            if (t1>t2) {float w = t1; t1=t2; t2=w; } // Swap
            if (t1>tMin) tMin = t1;
            if (t2<tMax) tMax = t2;

            // No intersection
            if (tMin > tMax) return false;
            if (tMax < 0) return false;
        }
        else {
            if (-e+boundingBoxMin.x > 0.0f || -e+boundingBoxMax.x < 0.0f)
            {
                return false;
            }
        }
    }

    {
        // We test each axis:
        // Y - axis
        glm::vec3 yaxis(modelMatrix[1].x, modelMatrix[1].y, modelMatrix[1].z);
        float e = glm::dot(yaxis, delta);
        float f = glm::dot(rayDirection, yaxis);

        // If f is near 0 do not do division
        if (fabs(f) > 0.0001f)
        {
            float t1 = (e+boundingBoxMin.y)/f;
            float t2 = (e+boundingBoxMax.y)/f;

            if (t1>t2) {float w = t1; t1=t2; t2=w; } // Swap
            if (t1>tMin) tMin = t1;
            if (t2<tMax) tMax = t2;

            // No intersection
            if (tMin > tMax) return false;
            if (tMax < 0) return false;
        }
        else {
            if (-e+boundingBoxMin.y > 0.0f || -e+boundingBoxMax.y < 0.0f)
            {
                return false;
            }
        }
    }

    {
        // We test each axis:
        // Z - axis
        glm::vec3 zaxis(modelMatrix[2].x, modelMatrix[2].y, modelMatrix[2].z);
        float e = glm::dot(zaxis, delta);
        float f = glm::dot(rayDirection, zaxis);

        // If f is near 0 do not do division
        if (fabs(f) > 0.0001f)
        {
            float t1 = (e+boundingBoxMin.z)/f;
            float t2 = (e+boundingBoxMax.z)/f;

            if (t1>t2) {float w = t1; t1=t2; t2=w; } // Swap
            if (t1>tMin) tMin = t1;
            if (t2<tMax) tMax = t2;

            // No intersection
            if (tMin > tMax) return false;
            if (tMax < 0) return false;
        }
        else {
            if (-e+boundingBoxMin.z > 0.0f || -e+boundingBoxMax.z < 0.0f)
            {
                return false;
            }
        }
    }
    
    distanceToHit = tMin;
    // All planes pass so we have an intersection
    return true;
}


// Scene Intersection function
bool Scene::CheckForIntersection(glm::vec3 rayOrigin, glm::vec3 rayDirection)
{
    float closest = INFINITY;
    void* object = nullptr;
    SceneType type;
    bool hit = false;

    // Check models
    for (auto& model : this->GetModelObjects())
    {
        float distanceToHit = 0;
        // Note here, models can be unselectable if set
        if (model->GetIsSelectable() && TestRayOBBIntersection(rayOrigin, rayDirection, model->GetBoundingBox()->getMin(), model->GetBoundingBox()->getMax(), model->GetModelMatrix(), distanceToHit))
        {
            if (distanceToHit < closest)
            {
                closest = distanceToHit;
                object = static_cast<void*>(model);
                type = SceneType::MODEL;
                hit = true;
            }
        }
    }

    // Check roads
    for (auto& road : this->GetRoadObjects())
    {
        float distanceToHit = 0;
        if (TestRayOBBIntersection(rayOrigin, rayDirection, road->GetRoadRenderer()->GetOBBMin(), road->GetRoadRenderer()->GetOBBMax(), road->GetModelMatrix(), distanceToHit))
        {
            if (distanceToHit < closest)
            {
                closest = distanceToHit;
                object = static_cast<void*>(road);
                type = SceneType::ROAD;
                hit = true;
            }
        }
    }

    // Check sprites
    

    // Check point lights


    // If we get a hit, apply which object is selected
    if (hit)
        sceneSelectedObject->Select(object, type);
    else 
        sceneSelectedObject->Deselect();

    return hit;
}

