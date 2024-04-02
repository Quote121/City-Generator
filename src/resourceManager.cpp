#include <resourceManager.hpp>

#include <glad/glad.h>
#include <fstream>

// Initalisation to nullptr
ResourceManager* ResourceManager::pinstance{nullptr};

ResourceManager* ResourceManager::getInstance()
{
    if (pinstance == nullptr)
    {
        pinstance = new ResourceManager();
    }
    return pinstance; 
}

void ResourceManager::deleteInstance()
{
    // Will call destructor
    delete(pinstance); 
    pinstance = nullptr;
}



ResourceManager::~ResourceManager() 
{
    LOG(STATUS, "========== On exit tally - Resource manager ==============")

    LOG(STATUS, "Shaders : " << shader_map.size())
    LOG(STATUS, "Textures: " << texture_map.size())
    LOG(STATUS, "Models  : " << model_map.size())

    LOG(STATUS, "========== On exit tally - Resource manager ==============")

    deleteShaders();
    LOG(STATUS, "Deleted shaders");
    deleteTextures();
    LOG(STATUS, "Deleted textures");
    deleteModels();
    LOG(STATUS, "Deleted models");
};

void ResourceManager::deleteShaders()
{
    for (auto& a : shader_map)
    {
        delete(a.second);
    }
}

void ResourceManager::deleteTextures()
{
    for (auto& a : texture_map)
    {
        delete(a.second);
    }
}

void ResourceManager::deleteModels()
{
    for (auto& a : model_map)
    {
        delete(a.second);
    }
}

std::string trim(const std::string& line)
{
    const char* WhiteSpace = " \t\v\r\n";
    std::size_t start = line.find_first_not_of(WhiteSpace);
    std::size_t end = line.find_last_not_of(WhiteSpace);
    return start == end ? std::string() : line.substr(start, end - start + 1);
}

void ResourceManager::PreLoadAssets(std::string& filePath)
{
    std::vector<std::string> modelPaths;
    std::vector<std::string> spritePaths;
    const int models = 1;
    const int sprites = 2;
    int mode = 0;

    // Open file stream read
    std::ifstream preLoadFile(filePath);

    if (!preLoadFile.is_open())
    {
        LOG(ERROR, "Failed to open pre-load asset file: " << filePath);
    }

    std::string line;
    // #Models
    while (std::getline(preLoadFile, line))
    {
        bool commented = false;

        // Remove white space from lines etc
        line = trim(line);
        if (line.size() == 0)
            continue;

        // Check if a mode
        if (line == "#Models")
        {
            mode = models;
            continue;
        }
        if (line == "#Sprites")
        {
            mode = sprites;
            continue;
        }

        // Check for a comment
        for (size_t i = 0; i < line.size(); i++)
        {
            char c = line[i];

            // A commnected line
            if (c == '/' && i != line.size() && line[i+1] == '/')
            {
                commented = true;
                break;
            }
        }

        // We add to which ever mode we are in
        if (!commented && mode != 0)
        {
            if (mode == models)
            {
                modelPaths.push_back(line);
            }
            if (mode == sprites)
            {
                spritePaths.push_back(line);
            }
        }
    }

    std::string directory = "../assets/";

    Shader* modelShader = this->LoadModelShader(nullptr, 0);

    // Load up everything
    for (auto& path : modelPaths)
    {
        this->LoadModel(directory + path, modelShader);
    }
    for (auto& path : spritePaths)
    {
        this->LoadTexture(path, true, &directory);
    }

    preLoadFile.close();
}



// Load shader
Shader* ResourceManager::LoadShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
    // We first check if its been loaded. If so return the pointer to it.
    if (shader_map.find(vertexShaderPath) != shader_map.end())
    {
        // Cache hit
        return shader_map.find(vertexShaderPath)->second;
    }
    // Cache miss
    else
    {
        Shader* shader = new Shader(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
        shader_map.insert(std::make_pair(vertexShaderPath, shader));
        return shader;
    }
}


// Load texture
TextureInfo* ResourceManager::LoadTexture(const std::string& textureName, bool flip_texture_vertically, const std::string* directory)
{
    std::string texturePath;

    // We have a directory 
    if (directory != nullptr)
    {
        texturePath = *directory + "/" + textureName;
    }
    else
    {
        texturePath = textureName;
    }

    // Check in map
    if (texture_map.find(texturePath) != texture_map.end())
    {
        // hit
        return texture_map.find(texturePath)->second;
    }
    // miss
    else
    {
        LOG(STATUS_SERV(LOG_RM), "Loading texture : " << texturePath); 
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
                format = GL_RGB;
                LOG(ERROR_SERV(LOG_RM), "ResourceManager::LoadTexture() image format unrecognised. nrComponents : " << nrComponents);
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
            LOG(ERROR_SERV(LOG_RM), "Texture failed to load at path : " << texturePath);
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
Model* ResourceManager::LoadModel(const std::string& modelPath_in, Shader* modelShader_in)
{
    // Hit
    if (model_map.find(modelPath_in) != model_map.end() )
    {
        Model* model = model_map.find(modelPath_in)->second;
        
        // Have to set the shader as we are only getting the model
        model->SetShader(modelShader_in);

        return model;
    }
    // Miss
    else
    {
        LOG(STATUS_SERV(LOG_RM), "Loading model : " << modelPath_in);
        Model* model = new Model(modelShader_in, modelPath_in);
        model_map.insert(std::make_pair(modelPath_in, model));
        return model;
    }
}


Shader* ResourceManager::LoadModelShader(const ShaderPath* shader_in, const bool instanced)
{
    
    // Default instance shader
    if (shader_in == nullptr && instanced)
    {
        return this->LoadShader(paths::object_defaultInstancedVertShaderPath, paths::object_defaultFragShaderPath);
    }
    // Default shader (not instanced)
    else if (shader_in == nullptr)
    {
        return this->LoadShader(paths::object_defaultVertShaderPath, paths::object_defaultFragShaderPath);
    }
    // Specified shader
    else 
    {
        return this->LoadShader(shader_in->vertex, shader_in->fragment);
    }
}

Shader* ResourceManager::LoadSpriteShader(const ShaderPath* shader_in, const bool instanced)
{
    // Default instance shader
    if (shader_in == nullptr && instanced)
    {
        return this->LoadShader(paths::sprite_defaultVertShaderPath, paths::sprite_defaultFragShaderPath);
    }
    // Default shader
    else if (shader_in == nullptr) 
    {
        return this->LoadShader(paths::sprite_defaultVertShaderPath, paths::sprite_defaultFragShaderPath);
    }
    // Specified shader
    else 
    {
        return this->LoadShader(shader_in->vertex, shader_in->fragment);
    }
}

Shader* ResourceManager::LoadRoadShader(const ShaderPath* shader_in)
{
    if (shader_in != nullptr)
    {
        return this->LoadShader(shader_in->vertex, shader_in->fragment);
    }
    else {
        return this->LoadShader(paths::road_defaultVertShaderPath, paths::road_defaultFragShaderPath);
    }
}


std::vector<Model*> ResourceManager::GetLoadedModels()
{
    std::vector<Model*> models;

    for (auto& a : model_map)
    {
        models.push_back(a.second);
    }
    return models;
}

std::vector<TextureInfo*> ResourceManager::GetLoadedTextures()
{
    std::vector<TextureInfo*> textures;
    for (auto& a : texture_map)
    {
        textures.push_back(a.second);
    }
    return textures;
}




