#include <renderer.hpp>
#include <config.hpp>
#include <glad/glad.h>

// Get roads
#include <scene.hpp>


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
    vbl.AddFloat(3); // xyz
    vbl.AddFloat(3); // normal


    // VBO->SetData<float>(, )
    // VAO->AddBuffer(VBO, &VBL);
    // EBO->SetData(, ROAD_MAX_INDICES)
    
    // No class for indirect buffer so manually here

    for (auto road : roads)
    {

    }

}

void Batch::Update(void)
{
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirectCommands.size() * sizeof(DrawElementsIndirectCommand));

}


void Batch::DrawBatch(void) const
{
    glDrawElementsIndirect(GL_TRIANGLES, 0, 0); 
}
















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
    glBindVertexArray(vao->GetVertexArray());
    glDrawElements(mode, ebo->GetCount(), GL_UNSIGNED_INT, nullptr); 

    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        LOG(ERROR, "OpenGL DrawIndices() Error: " << error);
    }
}

void Renderer::DrawArrays(const VertexArray* vao, unsigned int count, unsigned int mode)
{
    // glLineWidth(2.0f);
    glBindVertexArray(vao->GetVertexArray());
    glDrawArrays(mode, 0, count);

    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        LOG(ERROR, "OpenGL DrawArrays() Error: " << error);
    }
}
