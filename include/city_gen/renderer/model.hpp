#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb_image/stb_image.h>

#include <vector>
#include <string>

#include <mesh.hpp>

// Forward declarations
class BoundingBox;

/*
    Model class holds a vector of mesh objects.
    This is the class where we directly pass the model file to import into our project
*/
class Model
{
private:
    Shader* modelShader = nullptr;
    BoundingBox* modelBoundingBox;

    // model data
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> textures_loaded;

    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

public:
    Model(Shader* modelShader_in, const std::string& path);
    ~Model();

    inline Shader* GetShader()
    {
        return modelShader;
    }

    inline BoundingBox* GetBoundingBox()
    {
        return modelBoundingBox;
    }

    void Draw();
};
