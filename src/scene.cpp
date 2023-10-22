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
    scene_objects.push_back(dynamic_cast<BaseObject*>(model));
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
    scene_objects.push_back(dynamic_cast<BaseObject*>(sprite));
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
    scene_objects.push_back(dynamic_cast<BaseObject*>(line));
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

void Scene::drawSceneObjects(glm::mat4 view, glm::mat4 projection)
{
    for (BaseObject* sceneObj : scene_objects)
    {
        if (ModelObject* object = dynamic_cast<ModelObject*>(sceneObj))
        {
            object->Draw(view, projection);
        }
        else if (SpriteObject* object = dynamic_cast<SpriteObject*>(sceneObj))
        {
            object->Draw(view, projection);
        }
        // Line
        else if (LineObject* object = dynamic_cast<LineObject*>(sceneObj))
        {
            object->Draw(view, projection);
        }
        // // Light
        // else if (LightObject* object = dynamic_cast<LightObject*>(sceneObj))
        // {
        //     LOG(ERROR, "Light object not yet implemented");

        // }
        // Particle
        else if (ParticleObject* object = dynamic_cast<ParticleObject*>(sceneObj))
        {
            LOG(ERROR, "Particle object not yet implemented");
        }
        // Error
        else
        {
            LOG(ERROR, "Unrecognized object");
        }
    }
}





// // void Scene::addObject(const std::string &objectPath, Shader &shader, glm::vec3 pos_in, glm::vec3 scale_in, glm::vec3 colour_in)
// // {
// //     std::unique_ptr<Object> object = std::make_unique<Object>(objectPath, shader, pos_in, scale_in, colour_in);
// //     scene_objects.push_back(std::move(object));
// // }

// // For unique_ptr (will not work without passing the ownership)
// bool Scene::removeObject(std::unique_ptr<Object> &obj)
// {
//     // scene_objects.ge
//     auto result = std::find(scene_objects.begin(), scene_objects.end(), obj);
//     if (result == scene_objects.end()){
//         return false;
//     }
//     scene_objects.erase(result);
//     return true;
// }

// bool Scene::removeObject(Object &obj)
// {

//     auto it = std::find_if(scene_objects.begin(), scene_objects.end(), [&](const std::unique_ptr<Object>& ptr) {
//         return ptr.get() == &obj;
//     });

//     if (it == scene_objects.end())
//     {
//         std::cerr << "not found" << std::endl;
//         return false;
//     }
//     else{
//         std::cout << "Deleted: " << (*it)->getFilePath() << std::endl;
//         scene_objects.erase(it);
//         return true;
//     }
// }
