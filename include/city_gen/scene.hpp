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
//
////////////////
#pragma once

#include <vector>
#include <memory>
#include <algorithm>

#include <objects/all.hpp>
#include <skybox.hpp>

class Scene
{
private:
    // Scene objects specific to objects
    // This allows us to choose rendering order with minimal overhead
    std::vector<ModelObject*> scene_model_objects;
    std::vector<SpriteObject*> scene_sprite_objects;
    std::vector<LineObject*> scene_line_objects;

    std::vector<PointLightObject*> scene_pointLight_objects;
    std::vector<DirectionalLightObject*> scene_directionalLight_objects;

    SkyBox* skybox; // The skybox object


    Scene() {}

    ~Scene()
    {
        delete(pinstance_);
    }

    static Scene* pinstance_;

    // Used for alias'
    int lineCount = 0;
    int modelCount = 0;
    int spriteCount = 0;

    int dirLightCount = 0;
    int pointLightCount = 0;

    template<class T, class U>
    static bool SortByDistanceInv(BaseObject<T>* a, BaseObject<U>* b);

public:
    // Singleton setup //  
    Scene(Scene &other) = delete;
    void operator=(const Scene &) = delete;
    static Scene* getInstance();
    /////////////////////

    // 3D models
    void addModel(const ModelObject& modelObject);
    ModelObject* addModel(std::string& modelPath_in,
                          Shader* shader_in);

    // 2D sprites
    void addSprite(const SpriteObject& spriteObject);
    SpriteObject* addSprite(std::string& spriteTexture_in,
                            Shader *shader_in);

    // Line
    void addLine(const LineObject& lineObject);
    LineObject* addLine(Shader* shader_in,
                 glm::vec3 point_a,
                 glm::vec3 point_b,
                 glm::vec3 colour_in = glm::vec3{1.0f, 1.0f, 1.0f});

    // Light // UNUSED
    // LightObject* addLight(Shader* shader_in,
    //               LightType LightType_in);

    DirectionalLightObject* addDirectionalLight();
    PointLightObject* addPointLight();


    void CreateSkyBox(std::vector<std::string>* images);
    void DrawSkyBox(glm::mat4 view, glm::mat4 projection);


    // Particle

    // Returns true is found and removed, false otherwise
    template<class T>
    bool removeObject(BaseObject<T> &obj);
    bool removeModel(ModelObject& obj);
    bool removeSprite(SpriteObject& obj);
    bool removeLine(LineObject& obj);

    // Light
    // particle

    // std::vector<BaseObject*> const& getObjects()
    // {
    //     return scene_objects;
    // }

    // Get scene objects
    std::vector<ModelObject*> const& GetModelObjects()
    {
        return scene_model_objects;
    }

    std::vector<SpriteObject*> const& GetSpriteObjects()
    {
        // Sort by distance and return the sprites
        return scene_sprite_objects;
    }

    std::vector<LineObject*> const& GetLineObjects()
    {
        return scene_line_objects;
    }

    std::vector<PointLightObject*> const& GetPointLightObjects()
    {
        return scene_pointLight_objects;
    }

    std::vector<DirectionalLightObject*> const& GetDirectionalLightObjects()
    {
        return scene_directionalLight_objects;
    }

    // Draws all of the objects form each of the object vectors
    void drawSceneObjects(glm::mat4 view, glm::mat4 projection);
    
};
