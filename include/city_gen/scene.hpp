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
#include <shader.hpp>

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

    
    std::vector<LineObject*> scene_axis_lines;
    bool showSceneAxis = true;

    SkyBox* skybox; // The skybox object

    Scene();

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

    // Private axis add
    LineObject* addLineAxis(glm::vec3 point_a,
                            glm::vec3 point_b,
                            const ShaderPath* shader_in = nullptr);

public:
    // Singleton setup //  
    Scene(Scene &other) = delete;
    void operator=(const Scene &) = delete;
    static Scene* getInstance();
    /////////////////////

    // 3D models
    ModelObject* addModel(const std::string& modelPath_in,
                          const ShaderPath* shader_in = nullptr);

    // 2D sprites
    SpriteObject* addSprite(std::string& spriteTexture_in,
                            const ShaderPath* shader_in = nullptr);

    // Line
    LineObject* addLine(glm::vec3 point_a,
                        glm::vec3 point_b,
                        const ShaderPath* shader_in = nullptr);

    DirectionalLightObject* addDirectionalLight();
    PointLightObject* addPointLight();


    void CreateSkyBox(std::vector<std::string>* images);
    void DrawSkyBox(glm::mat4 view, glm::mat4 projection);


    // Particle

    // Returns true is found and removed, false otherwise
    void removeModel(ModelObject& obj);
    void removeSprite(SpriteObject& obj);
    void removeLine(LineObject& obj);
    void removePointLight(PointLightObject& obj);


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

    // Builders
    Scene* ShowSceneAxis(bool visible)
    {
        showSceneAxis = visible;
        return this;
    }

    // ImGui handle
    bool& GetShowSceneAxisImGui()
    {
        return showSceneAxis;
    }

    // Draws all of the objects form each of the object vectors
    void DrawSceneObjects(glm::mat4 view, glm::mat4 projection);
};
