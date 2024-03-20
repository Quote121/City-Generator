#include <mesh.hpp>

#include <shader.hpp>
#include <config.hpp>
#include <resourceManager.hpp>
#include <glad/glad.h>

void CheckGLError(const std::string& functionName) {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR)
    {
        std::cerr << "OpenGL Error after " << functionName << ": " << error << std::endl;
    }
}


Mesh::Mesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, std::vector<Texture> textures, Material material)
{
    glGenBuffers(1, &matrixBuffer);


    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    this->material = material;

    setupMesh();
}

Mesh::~Mesh()
{
    // NOT SURE HOW BUT THIS IS BEING CALLED WHEN IT SHOULDNT BE
    // LOG(STATUS, "~Mesh() called");
    // glDeleteVertexArrays(1, &VAO);
    // glDeleteBuffers(1, &VBO);
    // glDeleteBuffers(1, &EBO);

    // glDeleteBuffers(1, &matrixBuffer);
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // vertex pos
    glEnableVertexAttribArray(0); // layout = 0 in 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // vertex norm
    glEnableVertexAttribArray(1); // layout = 1 
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    // vertex texture coords
    glEnableVertexAttribArray(2); // layout = 2 
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));






    glBindVertexArray(0);
}

void Mesh::Draw(Shader &shader)
{

    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;

    // If we have textures, draw them
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // activate texture unit, with an offset
        // get textures number
        std::string number = "";
        std::string name = textures[i].type;
        if(name == "diffuse")
            number = std::to_string(diffuseNr++);
        else if(name == "specular")
            number = std::to_string(specularNr++);

        shader.setFloat(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0); // unbind


    // If we do not have textures then use the material
    if (textures.size() == 0)
    {
        // LOG(STATUS, "mat.amb " << material.ambience)
        // LOG(STATUS, "mat.dif " << material.diffuse)
        // LOG(STATUS, "mat.spec " << material.specular)
        // LOG(STATUS, "mat.shin " << material.shininess)
        //
        shader.setVec3("material.ambient", material.ambience);
        shader.setVec3("material.diffuse", material.diffuse);
        shader.setVec3("material.specular", material.specular);
        shader.setFloat("material.shininess", 10.0f);
    }

    // draw mesh
#if ENABLE_CULL_FACE_MODEL == 1
    glEnable(GL_CULL_FACE);
#endif
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    glBindTexture(GL_TEXTURE_2D, 0);

#if ENABLE_CULL_FACE_MODEL == 1
    glDisable(GL_CULL_FACE);
#endif

    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        LOG(ERROR, "OpenGL Mesh::Draw() Error: " << error);
    }
}

void Mesh::DrawInstanced(Shader &shader_unused, unsigned int instanceCount, std::vector<float>* matrices)
{
    Shader shader = *ResourceManager::getInstance()->LoadShader(paths::building_defaultInstancedVertShaderPath, paths::building_defaultFragShaderPath);

    // unsigned int diffuseNr = 1;
    // unsigned int specularNr = 1;
    //
    // // If we have textures, draw them
    // for (unsigned int i = 0; i < textures.size(); i++)
    // {
    //     glActiveTexture(GL_TEXTURE0 + i); // activate texture unit, with an offset
    //     // get textures number
    //     std::string number = "";
    //     std::string name = textures[i].type;
    //     if(name == "diffuse")
    //         number = std::to_string(diffuseNr++);
    //     else if(name == "specular")
    //         number = std::to_string(specularNr++);
    //
    //     shader.setFloat(("material." + name + number).c_str(), i);
    //     glBindTexture(GL_TEXTURE_2D, textures[i].id);
    // }
    // glActiveTexture(GL_TEXTURE0); // unbind
    //

    // If we do not have textures then use the material
    if (textures.size() == 0)
    {
        // LOG(STATUS, "mat.amb " << material.ambience)
        // LOG(STATUS, "mat.dif " << material.diffuse)
        // LOG(STATUS, "mat.spec " << material.specular)
        // LOG(STATUS, "mat.shin " << material.shininess)
        //
        shader.setVec3("material.ambient", material.ambience);
        shader.setVec3("material.diffuse", material.diffuse);
        shader.setVec3("material.specular", material.specular);
        shader.setFloat("material.shininess", 10.0f);
    }

    // draw mesh
#if ENABLE_CULL_FACE_MODEL == 1
    glEnable(GL_CULL_FACE);
#endif

    glBindVertexArray(VAO);
    // Bind matrices
    glBindBuffer(GL_ARRAY_BUFFER, matrixBuffer);
    glBufferData(GL_ARRAY_BUFFER, matrices->size() * sizeof(float), matrices->data(), GL_STATIC_DRAW);
    

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(4*sizeof(float)));

    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(8*sizeof(float)));

    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(12*sizeof(float)));

    // Configure them as instanced matrices
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);



    // glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr, instanceCount);
    
    glBindTexture(GL_TEXTURE_2D, 0);

#if ENABLE_CULL_FACE_MODEL == 1
    glDisable(GL_CULL_FACE);
#endif

    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        LOG(ERROR, "OpenGL Mesh::DrawInstanced() Error: " << error);
    }
}

