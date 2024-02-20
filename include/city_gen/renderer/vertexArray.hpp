#pragma once

#include <vertexBuffer.hpp>
#include <vertexBufferLayout.hpp>

class VertexArray
{
private:
    
    unsigned int VAO;

public:

    VertexArray(void);
    ~VertexArray(void);

    void AddBuffer(const VertexBuffer* vb, const VertexBufferLayout* layout);

    unsigned int GetVertexArray(void) const;

    void Bind(void) const;
    void Unbind(void) const;
};

