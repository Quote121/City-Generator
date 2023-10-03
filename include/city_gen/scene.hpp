/////////////////
//
// This class is for holding all current game objects etc. that are loaded into the scene
// It is responsible for holding the pointers to these objects as we dont want objects defined 
// the stack in the main function as it makes them hard to access by other classes.
// 
// This class is temporary until the scope of the engine is more defined and will be a singlton
// as currently there is no need for multiple scene classes.
//
//
//  todo future - thread safe access
//
//
////////////////
#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <memory>
#include <algorithm>

#include "object.hpp"

class Scene
{
private:
    std::vector<std::unique_ptr<Object>> scene_objects;

    Scene() {}

    ~Scene()
    {
        delete(pinstance_);
    }

    static Scene* pinstance_;

public:
    
    Scene(Scene &other) = delete;
    void operator=(const Scene &) = delete;
    static Scene* getInstance();

    void addObject(const std::string &objectPath, Shader &shader, glm::vec3 pos_in = glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3 scale_in = glm::vec3{1.0f, 1.0f, 1.0f}, glm::vec3 colour_in = glm::vec3{0.0f, 0.0f, 0.0f});

    // Returns true is found and removed
    // false otherwise
    bool removeObject(std::unique_ptr<Object> &obj);
    bool removeObject(Object &obj);

    std::vector<std::unique_ptr<Object>> const& getObjects() const
    {
        return scene_objects;
    }

    void drawObjects(glm::mat4 view, glm::mat4 projection, glm::vec4 colour = glm::vec4{0.0f, 0.0f, 0.0f, 1.0f})
    {
        for (auto& obj : scene_objects)
        {
            obj->Draw(view, projection, colour);
        }
    }
};

#endif // SCENE_HPP