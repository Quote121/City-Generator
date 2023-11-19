#include <scene.hpp>
#include <resourceManager.hpp>

Scene* Scene::pinstance_{nullptr};

Scene* Scene::getInstance()
{
    if (pinstance_ == nullptr)
    {
        pinstance_ = new Scene();
    }
    return pinstance_; 
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


// bool Scene::removeObject(BaseObject &obj)
// {
//     // TODO
// }

// bool Scene::removeModel(ModelObject& obj)
// {
//     // TODO
// }

// bool Scene::removeSprite(SpriteObject& obj)
// {
//     // TODO
// }

// BaseObject is a class template and so we have to specify the objects we are comparing
template<class T, class U>
bool Scene::SortByDistanceInv(BaseObject<T>* a, BaseObject<U>* b)
{   
    return a->GetDistanceFromCamera() > b->GetDistanceFromCamera();
}

void Scene::drawSceneObjects(glm::mat4 view, glm::mat4 projection)
{
    // Draw lines
    for (auto& line : GetLineObjects())
    {
        line->Draw(view, projection);
    }

    // Draw objects
    for (auto& obj : GetModelObjects())
    {
        obj->Draw(view, projection);
    }

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
    skybox->Draw(view, projection);
}
