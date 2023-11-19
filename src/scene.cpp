#include <scene.hpp>

Scene* Scene::pinstance_{nullptr};

Scene* Scene::getInstance()
{
    if (pinstance_ == nullptr)
    {
        pinstance_ = new Scene();
    }
    return pinstance_; 
}

void Scene::addModel(const ModelObject& modelObject)
{
    // TODO
}

ModelObject* Scene::addModel(std::string& modelPath_in,
                             Shader* shader_in)
{
    ModelObject* model = new ModelObject(
        modelPath_in, shader_in
    );
    scene_model_objects.push_back(model);

    modelCount++;

    std::string name = "Model_" + std::to_string(modelCount);
    model->SetAlias(&name);
    
    return model;
}

void Scene::addSprite(const SpriteObject& spriteObject)
{
    // TODO
}

SpriteObject* Scene::addSprite(std::string& spriteTexture_in,
                Shader *shader_in)
{
    // Create sprite, allocate memory and put in list
    SpriteObject* sprite = new SpriteObject(
        spriteTexture_in, shader_in
    );
    scene_sprite_objects.push_back(sprite);
    
    spriteCount++;

    std::string name = "Sprite_" + std::to_string(spriteCount);
    sprite->SetAlias(&name);
    
    return sprite;
}


void Scene::addLine(const LineObject& LineObject)
{
    // TODO
}


LineObject* Scene::addLine(Shader* shader_in,
               glm::vec3 point_a,
               glm::vec3 point_b,
               glm::vec3 colour_in)
{
    // Create line
    LineObject* line = new LineObject(
        shader_in, point_a, point_b, colour_in
    );
    scene_line_objects.push_back(line);

    lineCount++;

    std::string name = "Line_" + std::to_string(lineCount);
    line->SetAlias(&name);

    return line;
}

// MIGHT BE UNUSED
// LightObject* Scene::addLight(Shader* shader_in,
//                              LightType lightTypeIn)
// {
    
//     // LightObject* light = new LightObject(
//     //     shader_in, lightTypeIn
//     // );
//     // // scene_light_objects.push_back(light);

//     // lightCount++;

//     // // Concatenate the light type onto the alias
//     // std::string lightTypeStr = LightObject::GetStringLightType(lightTypeIn);

//     // std::string name = "Light_" + lightTypeStr + "_" + std::to_string(lightCount);
//     // light->SetAlias(&name);

//     // return light;

// }

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
