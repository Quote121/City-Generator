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

#include <renderer.hpp>
#include <objects/all.hpp>
#include <skybox.hpp>
#include <shader.hpp>

#include <vector>

// SceneObject types
typedef enum class e_SceneType
{
    MODEL,
    SPRITE,
    LINE,
    ROAD,
    P_LIGHT,
    D_LIGHT
} SceneType;

// Type used for object selection
class SelectedObject
{
private:
    bool isSelected = false; // boolean used to check if the data is valid or garbage
    void* object = nullptr;  // pointer to the class object
    SceneType type;          // type of class for casting

public:
    SelectedObject() = default;

    bool HasObjectSelected(void) const
    {
        return isSelected;
    }

    void Select(void* object, SceneType type)
    {
        this->object = object;
        this->type = type;
        isSelected = true;
    }

    void Deselect(void)
    {
        isSelected = false;
    }

    void* GetObject(void)
    {
        return object;
    }

    SceneType GetType(void)
    {
        return type;
    }

    void Draw()
    {
        if (type == SceneType::MODEL)
        {
            static_cast<ModelObject*>(object)->DrawBoundingBox(WHITE);
        }
        else if (type == SceneType::ROAD)
        {
            static_cast<RoadObject*>(object)->DrawBoundingBox(WHITE);
        }
        else if (type == SceneType::SPRITE)
        {
            // static_cast<SpriteObject*>(object);
        }
    }
};


class Scene
{
private:
    // Scene objects specific to objects
    // This allows us to choose rendering order with minimal overhead
    std::vector<ModelObject*> scene_model_objects;
    std::vector<SpriteObject*> scene_sprite_objects;
    std::vector<LineObject*> scene_line_objects;
    std::vector<RoadObject*> scene_road_objects;

    std::vector<PointLightObject*> scene_pointLight_objects;
    std::vector<DirectionalLightObject*> scene_directionalLight_objects;

    std::vector<LineObject*> scene_axis_lines;
    bool showSceneAxis = true;
    bool showSkybox = true;
    bool showTerrain = true;
    bool showRoadZones = true; // TODO REDUNDANT?
    bool removeIntersectingZones = false; // TODO REDUNDANT? 
    // If a road zone has been labled as cannot be used (intersect then dont render it)

    // All possible skyboxes 
    std::vector<SkyBox*> skyboxes;

    SkyBox* selectedSkybox = nullptr;       // The skybox object
    ModelObject* terrain; // Terrain object

    Scene();
    ~Scene();

    static Scene* pInstance;

    // Used for alias'
    int lineCount = 0;
    int modelCount = 0;
    int spriteCount = 0;
    int roadCount = 0;

    int dirLightCount = 0;
    int pointLightCount = 0;

    // Instance renderers
    std::vector<InstanceRenderer<ModelObject*>*> modelInstanceRenderers;
    // std::vector<InstanceRenderer<const SpriteObject*>*> spriteInstanceRenderers;
 

    // Methods to add objects to instance renderers
    void addModelToInstanceRenderer(ModelObject* modelObject_in,
                                    const std::string& modelPath_in,
                                    const ShaderPath* shader_in);
    void addSpriteToInstanceRenderer(SpriteObject const* SpriteObject_in,
                                     const std::string& spriteTexture_in,
                                     const ShaderPath* shader_in);

    template<class T, class U>
    static bool SortByDistanceInv(BaseObject<T>* a, BaseObject<U>* b);

    // Private axis add
    LineObject* addLineAxis(glm::vec3 point_a,
                            glm::vec3 point_b,
                            const ShaderPath* shader_in = nullptr);
public:

    // Only batch renderer for roads
    BatchRenderer* roadBatchRenderer;

    // Currently selected object 
    SelectedObject* sceneSelectedObject;

    // Singleton setup //  
    Scene(Scene &other) = delete;
    void operator=(const Scene &) = delete;
    static Scene* getInstance();
    /////////////////////

    ModelObject* addTerrain(const std::string& modelPath_in,
                            const ShaderPath* shader_in = nullptr);

    // 3D models
    ModelObject* addModel(const std::string& modelPath_in,
                          const ShaderPath* shader_in = nullptr,
                          const bool instanced = false);

    // 2D sprites
    SpriteObject* addSprite(std::string& spriteTexture_in,
                            const ShaderPath* shader_in = nullptr,
                            const bool instanced = false);

    // Line
    LineObject* addLine(glm::vec3 point_a,
                        glm::vec3 point_b,
                        const ShaderPath* shader_in = nullptr);

    // Roads
    RoadObject* addRoad(glm::vec3 point_a,
                        glm::vec3 point_b,
                        float road_width = 1.0f, // default road width size
                        const ShaderPath* shader_in = nullptr);

    DirectionalLightObject* addDirectionalLight();
    PointLightObject* addPointLight();

    // @brief sometimes we need to reload all the matrix data of all objects
    void ForceReloadInstanceRendererData(void) const;

    // @brief will update models if its instanced or not
    void UpdateModelInstanced(ModelObject* const object) const;
    void UpdateSpriteInstanced(SpriteObject* object) const;

    void SetSkybox(const int index)
    {
        selectedSkybox = skyboxes[index];
    }
    void LoadSkyboxes(void);

    // @brief Called after a shader is bound to setup all lights for a shader
    // @param shader being used, should be bound before call
    void SetShaderLights(const Shader* shader);

    // Check for intersection
    bool CheckForIntersection(glm::vec3 rayOrigin, glm::vec3 rayDirection);

    // Remove individual objects
    void removeModel(ModelObject& obj);
    void removeSprite(SpriteObject& obj);
    void removeLine(LineObject& obj);
    void removePointLight(PointLightObject& obj);
    void removeDirectionalLight(DirectionalLightObject& obj);
    void removeRoad(RoadObject& obj);

    // Clear types of objects (delete them all)
    void removeAllModels(void);
    void removeAllSprites(void);
    void removeAllLines(void);
    void removeAllPointLights(void);
    void removeAllDirectionalLights(void);
    void removeAllRoads(void);
    void removeAllInstanceRenderers(void);

    std::vector<SkyBox*> const& GetSkyBoxes()
    {
        return skyboxes;
    }

    // Get scene objects
    std::vector<ModelObject*> const& GetModelObjects()
    {
        return scene_model_objects;
    }

    std::vector<RoadObject*> const& GetRoadObjects()
    {
        return scene_road_objects;
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

    bool& GetShowSkyBoxImGui()
    {
        return showSkybox;
    }

    bool& GetShowTerrainImGui()
    {
        return showTerrain;
    }

    bool& GetShowRoadZones()
    {
        return showRoadZones;
    }

    bool& GetRemoveIntersectingZones()
    {
        return removeIntersectingZones;
    }


    // Draws all of the objects form each of the object vectors
    void DrawScene(void);
};
