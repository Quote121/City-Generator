#include <renderer.hpp>
#include <config.hpp>
#include <glad/glad.h>

// Get roads
#include <scene.hpp>
#include <cassert>
#include <camera.hpp>
#include <resourceManager.hpp>

void checkGLError(const std::string& functionName) {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR)
    {
        std::cerr << "OpenGL Error after " << functionName << ": " << error << std::endl;
    }
}

Batch::Batch()
{
    VAO = new VertexArray();
    VBO = new VertexBuffer();
    EBO = new IndexBuffer();
    glGenBuffers(1, &IBO);
}

Batch::~Batch()
{
    delete(VAO);
    delete(VBO);
    delete(EBO);
    glDeleteBuffers(1, &IBO);
}

void Batch::UpdateAll(void)
{
    // Gives new renderIDs

    // Clear out the vector
    if (!indirectCommands.empty())
    {
        indirectCommands.clear();
    }

    auto roads = Scene::getInstance()->GetRoadObjects();

    // Create buffers based on this size
 

    // VBO bytes -> number of roads * maxvertices * 3(xyz) * sizeof(float)
    // EBO bytes -> sizeof(unsigned int) * maxindices
    VertexBufferLayout vbl;
    vbl.AddFloat(3); // aPos
    vbl.AddFloat(3); // normal

    // Buffer for all roads
    VBO->CreateBuffer(ROAD_MAX_BUFFER_SIZE_BYTES * roads.size());

    // count, _, offset, _, _
    // DrawElementsIndirectCommand edic = {8, 0, 1, 0, 0};

    std::vector<unsigned int> updatedIndices;
    
    // No class for indirect buffer so manually here
    for (unsigned int i = 0; i < roads.size(); i++)
    {
        const auto roadRenderer = roads[i]->GetRoadRenderer();
        unsigned int indexCount = roads[i]->GetRoadRenderer()->GetIndices()->size();
        
        roads[i]->GetRoadRenderer()->SetBatchRenderID(i);
        int renderID = i;

        // Check that the road is valid
        assert(indexCount <= ROAD_MAX_INDICES);
        assert(renderID >= 0);

        // Enter data into VBO and EBO
        VBO->UpdateBuffer(roadRenderer->GetVertices()->data(), i * ROAD_MAX_BUFFER_SIZE_BYTES, roadRenderer->GetVertices()->size() * sizeof(float));
        // EBO NEEDS TO BE UPDATED WITH THE VERTEX INDEX FOR EACH ROAD, THEY WILL BE ALL STARING AT 0
        
        // For each index in roadRenderer, itterate by ROAD_MAX_INDICES*i
        for (auto a : *roadRenderer->GetIndices())
        {
            updatedIndices.push_back(a += (i*ROAD_MAX_VERTICES));
        }
    }
    EBO->SetData(updatedIndices.data(), updatedIndices.size());

    // Bind it all to VAO
    VAO->AddBuffer(VBO, &vbl);
    
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        LOG(ERROR, "OpenGL UpdateAll() Error: " << error);
    }
}

void Batch::Update(const unsigned int renderID)
{
    // TODO how to nicly pass the vertex and element data and renderID

    // Update vertices
    VAO->Bind();
    glBufferSubData(GL_VERTEX_ARRAY, 0, 0, 0);

    // Update indicies
    EBO->Bind();
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 0, 0);

    // Bind VAO
    // Bind VBO to VAO
    // Bind EBO to VAO
    // Bind IBO to VAO
    
    // Update indirect buffer
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, IBO);
    glBufferData(GL_DRAW_INDIRECT_BUFFER, indirectCommands.size() * sizeof(DrawElementsIndirectCommand), indirectCommands.data(), GL_STATIC_DRAW); // Might change flag (buffer is updated)
}


void Batch::DrawBatch(glm::mat4 view, glm::mat4 projection) const
{
    glm::mat4 result = glm::mat4(1.0f);
    Shader* objectShader = ResourceManager::getInstance()->LoadShader(paths::road_defaultVertShaderPath, paths::road_defaultFragShaderPath);
    
    objectShader->use();
    objectShader->setMat4("view", view);
    objectShader->setMat4("projection", projection);
    objectShader->setMat4("model", result); // TODO remove from shader as we cannot translate/rotate/scale the model
    objectShader->setVec3("colour", DEFAULT_ROAD_COLOUR); 

    objectShader->setBool("ShowLighting", true);

    // Lighting
    if (true)
    {
        // TEMP we go through all point lights and assign the values form it here
        objectShader->setVec3("viewPos", Camera::getInstance()->Position);
        objectShader->setFloat("material.shininess", 5.0f);
        
        size_t pointLightSize = Scene::getInstance()->GetPointLightObjects().size();
        
        objectShader->setInt("NumValidPointLights", pointLightSize);

        // For each point light set the corresponding values
        for (long unsigned int i = 0; i < pointLightSize; i++)
        {
            auto& light = Scene::getInstance()->GetPointLightObjects().at(i);
            std::string lightName = "pointLights[" + std::to_string(i) + "]";
            
            objectShader->setVec3((lightName + ".position"), light->GetPosition());
            objectShader->setVec3((lightName + ".ambient"), light->GetAmbient());
            objectShader->setVec3((lightName + ".diffuse"), light->GetDiffuse());
            objectShader->setVec3((lightName + ".specular"), light->GetSpecular());
            objectShader->setFloat((lightName + ".constant"), light->GetConstant());
            objectShader->setFloat((lightName + ".linear"), light->GetLinear());
            objectShader->setFloat((lightName + ".quadratic"), light->GetQuadratic());
        }

        // Directional lights
        DirectionalLightObject* dirLight = Scene::getInstance()->GetDirectionalLightObjects().at(0);
        objectShader->setVec3(("dirLight.direction"), dirLight->GetDirection());

        objectShader->setVec3(("dirLight.ambient"), dirLight->GetAmbient());
        objectShader->setVec3(("dirLight.diffuse"), dirLight->GetDiffuse());
        objectShader->setVec3(("dirLight.specular"), dirLight->GetSpecular());
    }

    // Bind all relevant buffers before draw
    VAO->Bind();
    EBO->Bind();
    glDrawElements(GL_TRIANGLES, EBO->GetCount(), GL_UNSIGNED_INT, nullptr);
    // glBindBuffer(GL_DRAW_INDIRECT_BUFFER, IBO);
    // glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, indirectCommands.size(), 0);
    // glDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr); // nullptr uses whatever is bound to GL_DRAW_INDIRECT_BUFFER

    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        LOG(ERROR, "OpenGL DrawBatch() Error: " << error);
    }

}

///////////////
//
// Renderer
//
///////////////

Renderer* Renderer::pInstance{nullptr};

Renderer* Renderer::GetInstance()
{
    if (pInstance == nullptr)
    {
        pInstance = new Renderer();
    }
    return pInstance; 
}

void Renderer::ClearScreen(void) const
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::DrawIndices(const VertexArray* vao, const IndexBuffer* ebo, unsigned int mode)
{
    vao->Bind();
    ebo->Bind(); 

    glDrawElements(mode, ebo->GetCount(), GL_UNSIGNED_INT, nullptr); 

    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        LOG(ERROR, "OpenGL DrawIndices() Error: " << error);
    }
}

void Renderer::DrawArrays(const VertexArray* vao, unsigned int count, unsigned int mode)
{
    // glLineWidth(2.0f);
    vao->Bind();
    glDrawArrays(mode, 0, count);

    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        LOG(ERROR, "OpenGL DrawArrays() Error: " << error);
    }
}



