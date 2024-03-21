#pragma once

/*

    This header is for managing assets throughout the program

    Please note that the deletion methods in the resourceManager are purely for assets such as object files and textures
    Not the scene object, that is dealt with by the scene class in scene.hpp 

    By loading all assets once into memory we can save on memory and startup time
    This will manage textures, shaders, objects

*/

// Unordered map uses buckets
#include <unordered_map>
#include <stb_image/stb_image.h>        // image reading

#include <config.hpp>                   // logging
#include <shader.hpp>
#include <model.hpp>

// Macro for logging of this file
#define LOG_RM "RESOURCEMANAGER"
       
// Texture struct
struct TextureInfo{
    unsigned int textureID; // Texture ID to bind
    int width;              // width of texture
    int height;             // height of texture
    std::string fileName;   // Name of texture
};


class ResourceManager{
private:
    // Since we are storing these as pointers we will need to deallocate all these resources upon exit
    // the resourceManager should have a cleanup feature

    // For shaders we will use the vertex shader path as the lookup
    std::unordered_map<std::string, Shader*> shader_map;

    // Seach with texture path and return texture struct
    std::unordered_map<std::string, TextureInfo*> texture_map;

    // Search with model path e.g. box.obj. Return model underlying loaded model
    std::unordered_map<std::string, Model*> model_map;

    static ResourceManager* pinstance;
    ResourceManager() {};
    ~ResourceManager(); 
    
    void deleteShaders();
    void deleteTextures();
    void deleteModels();

public:
    ResourceManager(ResourceManager &other) = delete;
    void operator=(const ResourceManager &) = delete;

    static ResourceManager* getInstance();
    static void deleteInstance();
    
    void PreLoadAssets(std::string& filePath);
    

    // Load shader
    Shader* LoadShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath); 

    // Load texture
    TextureInfo* LoadTexture(const std::string& textureName, bool flip_texture_vertically, const std::string* directory = nullptr);
    
    // Load model
    Model* LoadModel(const std::string& modelPath_in, Shader* modelShader_in);

    Shader* LoadModelShader(const ShaderPath* shader_in);
    Shader* LoadSpriteShader(const ShaderPath* shader_in); 
    Shader* LoadRoadShader(const ShaderPath* shader_in); 


    std::vector<Model*> GetLoadedModels();
    std::vector<TextureInfo*> GetLoadedTextures(); 
};

