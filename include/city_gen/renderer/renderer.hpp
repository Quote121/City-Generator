#pragma once

#include "indexBuffer.hpp"
#include "vertexArray.hpp"

// class Batch;

// Defined outside of Glad as we cant include several times
#define GL_TRIANGLES 0x0004

class Renderer
{
private:

    // Batches for rendering
    // Each batch contains a VAO, VBO, EBO and shader, they all share the same shader
    // Each object in the batch should know which batch they are in. And where in the buffers they are
    // An object can directly access and update the batch
    // std::vector<Batch> Batches;

    static Renderer* pInstance;  

    Renderer() = default;
    ~Renderer();

public:
    // Singleton
    Renderer(Renderer &other) = delete;
    void operator=(const Renderer &) = delete;
    static Renderer* GetInstance();


    void ClearScreen(void) const;
    // Make sure shaders are bound before calls to draw, mode is the primative to draw, default gl_triangle
    void DrawIndices(const VertexArray* vao, const IndexBuffer* ebo, unsigned int mode = GL_TRIANGLES);
    // @params count - number of arrays to draw with primative from vao
    void DrawArrays(const VertexArray* vao, unsigned int count, unsigned int mode = GL_TRIANGLES);

    void DrawBatches();
};

