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


void Scene::addModel(std::string& modelPath_in,                          
                Shader *shader_in,                                   
                float isVisible_in = true,
                glm::vec3 pos_in = glm::vec3{0.0f, 0.0f, 0.0f},      
                glm::vec3 rot_in = glm::vec3{0.0f, 0.0f, 0.0f},       
                glm::vec3 scl_in = glm::vec3{1.0f, 1.0f, 1.0f})
{
    // Create model and allocate memory for it, and add it to the scene
    ModelObject* model = new ModelObject(
        modelPath_in, shader_in, isVisible_in, pos_in, rot_in, scl_in
    );
    scene_objects.push_back(dynamic_cast<BaseObject*>(model));
}


void Scene::addObject(const std::string &objectPath, Shader &shader, glm::vec3 pos_in, glm::vec3 scale_in, glm::vec3 colour_in)
{
    std::unique_ptr<Object> object = std::make_unique<Object>(objectPath, shader, pos_in, scale_in, colour_in);
    scene_objects.push_back(std::move(object));
}

// For unique_ptr (will not work without passing the ownership)
bool Scene::removeObject(std::unique_ptr<Object> &obj)
{
    // scene_objects.ge
    auto result = std::find(scene_objects.begin(), scene_objects.end(), obj);
    if (result == scene_objects.end()){
        return false;
    }
    scene_objects.erase(result);
    return true;
}

bool Scene::removeObject(Object &obj)
{

    auto it = std::find_if(scene_objects.begin(), scene_objects.end(), [&](const std::unique_ptr<Object>& ptr) {
        return ptr.get() == &obj;
    });

    if (it == scene_objects.end())
    {
        std::cerr << "not found" << std::endl;
        return false;
    }
    else{
        std::cout << "Deleted: " << (*it)->getFilePath() << std::endl;
        scene_objects.erase(it);
        return true;
    }
}
