#include <mesh.hpp>

#include <shader.hpp>
#include <config.hpp>


Mesh::Mesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, std::vector<Texture> textures, Material material)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    this->material = material;

    setupMesh();
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
