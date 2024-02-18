#pragma once

#include "indexBuffer.hpp"
#include "vertexArray.hpp"

// class Batch;

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


    void ClearScreen();
    // Make sure shaders are bound before calls to draw
    void DrawIndices(const VertexArray& vao, const IndexBuffer& ebo);
    // @params count - number of arrays to draw with primative from vao
    void DrawArrays(const VertexArray& vao, unsigned int count);
    void DrawBatches();
};

