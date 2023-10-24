#include "scene.hpp"


Scene* Scene::pinstance_{nullptr};

Scene* Scene::getInstance()
{
    if (pinstance_ == nullptr)
    {
        pinstance_ = new Scene();
    }
    return pinstance_; 
}

void Scene::addObject(BaseObject* object_in) const
{
    // TODO
}

void Scene::addModel(const ModelObject& modelObject)
{
    // TODO
}

// ModelObject* Scene::addModel(std::string& modelPath_in,                          
//                      Shader *shader_in,                                   
//                      bool isVisible_in,
//                      glm::vec3 pos_in,      
//                      glm::vec3 rot_in,       
//                      glm::vec3 scl_in)
// {
//     // Create model and allocate memory for it, and add it to the scene
//     ModelObject* model = new ModelObject(
//         modelPath_in, shader_in, isVisible_in, pos_in, rot_in, scl_in
//     );
//     scene_objects.push_back(dynamic_cast<BaseObject*>(model));
//     return model;
// }

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
    // TOOD
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

bool Scene::removeObject(BaseObject &obj)
{
    // TODO
}

bool Scene::removeModel(ModelObject& obj)
{
    // TODO
}

bool Scene::removeSprite(SpriteObject& obj)
{
    // TODO
}

bool Scene::SortByDistanceInv(BaseObject* a, BaseObject* b)
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

    std::sort(scene_sprite_objects.begin(), scene_sprite_objects.end(), SortByDistanceInv);
    // Draw sprites
    for (auto& sprite : GetSpriteObjects())
    {
        sprite->Draw(view, projection);
    }
}
