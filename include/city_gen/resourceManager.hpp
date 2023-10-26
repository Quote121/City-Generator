#pragma once

/*

    This header is for managing assets throughout the program

    By loading all assets once into memory we can save on memory and startup time
    This will manage textures, shaders, objects

*/

// Unordered map uses buckets
#include <unordered_map>
#include <stb_image/stb_image.h>        // image reading
#include <glad/glad.h>                  // opengl bindings

#include "config.hpp"                   // logging
#include "model.hpp"
#include "shader.hpp"

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
    ~ResourceManager() 
    {
        delete(pinstance);
    };


public:
    ResourceManager(ResourceManager &other) = delete;
    void operator=(const ResourceManager &) = delete;
    static ResourceManager* getInstance()
    {
        if (pinstance == nullptr)
        {
            pinstance = new ResourceManager();
        }
        return pinstance; 
    }



    // Load shader
    Shader* LoadShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
    {
        // We first check if its been loaded. If so return the pointer to it.
        if (shader_map.find(vertexShaderPath) != shader_map.end())
        {
            // Cache hit
            LOG(STATUS, "LoadShader cache HIT");
            return shader_map.find(vertexShaderPath)->second;
        }
        // Cache miss
        else
        {
            LOG(STATUS, "LoadShader cache MISS");
            Shader* shader = new Shader(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
            shader_map.insert(std::make_pair(vertexShaderPath, shader));
            return shader;
        }
    }

    // Load texture
    TextureInfo* LoadTexture(const std::string& texturePath, bool flip_texture_vertically)
    {
        // Check in map
        if (texture_map.find(texturePath) != texture_map.end())
        {
            // hit
            LOG(STATUS, "LoadTexture cache HIT");
            return texture_map.find(texturePath)->second;
        }
        // miss
        else
        {
            LOG(STATUS, "LoadTexture cache MISS");
            unsigned int textureID;
            glGenTextures(1, &textureID);

            int width, height, nrComponents;

            stbi_set_flip_vertically_on_load(flip_texture_vertically);
            unsigned char *data = stbi_load(texturePath.c_str(), &width, &height, &nrComponents, 0);
            if (data)
            {
                // Channels in file then we set the format
                GLenum format;
                if (nrComponents == 1)
                    format = GL_RED;
                else if (nrComponents == 3)
                    format = GL_RGB;
                else if (nrComponents == 4)
                    format = GL_RGBA;
                else 
                {
                    LOG(ERROR, "ResourceManager::LoadTexture() image format unrecognised. nrComponents : " << nrComponents);
                }
    
                glBindTexture(GL_TEXTURE_2D, textureID);
                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            }
            else
            {
                LOG(ERROR, "Texture failed to load at path : " << texturePath);
            }
            stbi_image_free(data);
            
            // Create struct
            TextureInfo* texInfo = new TextureInfo();
            texInfo->textureID = textureID;
            texInfo->width = width;
            texInfo->height = height;
            texInfo->fileName = texturePath;
            
            // Insert into map and return the struct
            texture_map.insert(std::make_pair(texturePath, texInfo));
            return texInfo;
        }
    }

    // Load model
    Model* LoadModel(const std::string& modelPath_in, Shader* modelShader_in, BoundingBox* boundingBox_in)
    {
        // Hit
        if (model_map.find(modelPath_in) != model_map.end())
        {
            return model_map.find(modelPath_in)->second;
        }
        // Miss
        else
        {
            Model* model = new Model(modelShader_in, modelPath_in, boundingBox_in);
            model_map.insert(std::make_pair(modelPath_in, model));
            return model;
        }
    }
};
