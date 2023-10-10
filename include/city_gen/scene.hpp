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
//  TODO thread safe access
//
////////////////
#pragma once

#include <vector>
#include <memory>
#include <algorithm>

#include "config.hpp"
#include "objects/all.hpp"


class Scene
{
private:
    std::vector<BaseObject*> scene_objects;

    Scene() {}

    ~Scene()
    {
        delete(pinstance_);
    }

    static Scene* pinstance_;

public:
    // Singleton setup //  
    Scene(Scene &other) = delete;
    void operator=(const Scene &) = delete;
    static Scene* getInstance();
    /////////////////////

    // Object adding
    // When we create from a derived class, we downcast to base when storing
    void addObject(BaseObject* object_in) const;

    void addModel(const ModelObject& modelObject);
    void addModel(std::string& modelPath_in,                          
                  Shader *shader_in,                                   
                  float isVisible_in = true,
                  glm::vec3 pos_in = glm::vec3{0.0f, 0.0f, 0.0f},      
                  glm::vec3 rot_in = glm::vec3{0.0f, 0.0f, 0.0f},       
                  glm::vec3 scl_in = glm::vec3{1.0f, 1.0f, 1.0f});

    void addSprite(const SpriteObject& spriteObject);
    void addSprite(std::string& spriteTexture_in,
                 Shader *shader_in,
                 float isVisible_in = true,
                 bool isBillboard_in = false,
                 glm::vec3 pos_in = glm::vec3{0.0f, 0.0f, 0.0f},      // Starting pos of the object, defualt origin
                 glm::vec3 rot_in = glm::vec3{0.0f, 0.0f, 0.0f},      // 
                 glm::vec2 scl_in = glm::vec2{1.0f, 1.0f});

    // Light
    // Particle
    // Line

    void addObject(const std::string &objectPath, Shader &shader, glm::vec3 pos_in = glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3 scale_in = glm::vec3{1.0f, 1.0f, 1.0f}, glm::vec3 colour_in = glm::vec3{0.0f, 0.0f, 0.0f});

    // Returns true is found and removed, false otherwise
    bool removeObject(BaseObject &obj);
    bool removeModel(ModelObject& obj);
    bool removeSprite(SpriteObject& obj);

    // Light
    // particle
    // light

    std::vector<BaseObject*> const& getObjects()
    {
        return scene_objects;
    }

    // Casts each of the objects into their respected derived class to then call their respected draw functions
    void drawSceneObjects(glm::mat4 view, glm::mat4 projection)
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
                LOG(ERROR, "Line object not yet implemented");
            }
            // Lught
            else if (LightObject* object = dynamic_cast<LightObject*>(sceneObj))
            {
                LOG(ERROR, "Light object not yet implemented");

            }
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
};