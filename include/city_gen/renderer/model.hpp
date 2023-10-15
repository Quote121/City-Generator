#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <string>

#include "mesh.hpp"
#include "bounding_box.hpp" // for passing every vertex through to determine box


/*
    Model class holds a vector of mesh objects.
    This is the class where we directly pass the model file to import into our project
*/
class Model
{
private:
    Shader* modelShader = nullptr;

    BoundingBox* boundingBox;

    // model data
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> textures_loaded;

    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

    // Path is relative, default directory is current to the executable
    unsigned int TextureFromFile(const char *path, const std::string &directory = ".", bool gamma = false);
    
public:
    void Draw();

    Shader* GetModelShader();

    Model(Shader* modelShader_in, const std::string& path, BoundingBox* boundingBox_in);
};
