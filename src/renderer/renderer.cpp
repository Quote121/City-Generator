#include <renderer.hpp>
#include <config.hpp>
#include <glad/glad.h>

// Get roads
#include <scene.hpp>
#include <cassert>

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

    // Buffer for all roads
    VBO->CreateBuffer(ROAD_MAX_BUFFER_SIZE_BYTES * roads.size());
    EBO->CreateBuffer(ROAD_MAX_INDICES * roads.size());

    vbl.AddFloat(3); // aPos
    vbl.AddFloat(3); // normals
    
    // count, _, offset, _, _
    DrawElementsIndirectCommand edic = {8, 0, 1, 0, 0};

    // No class for indirect buffer so manually here
    for (auto road : roads)
    {
        unsigned int indexCount = road->GetRoadRenderer()->GetIndices()->size();
        int renderID = road->GetRoadRenderer()->GetBatchRenderID();

        // Check that the road is valid
        assert(indexCount <= ROAD_MAX_INDICES);
        assert(renderID >= 0);

        indirectCommands.push_back({indexCount, 0, renderID * ROAD_MAX_BUFFER_SIZE_BYTES, 0, 0});
        
        // Enter data into VBO and EBO
    }

    // Bind it all to VAO

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
