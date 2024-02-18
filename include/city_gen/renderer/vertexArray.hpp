#pragma once

#include <vertexBuffer.hpp>
#include <vertexBufferLayout.hpp>

class VertexArray
{
private:
    
    unsigned int VAO;
    // type primative
    unsigned int type;

public:

    VertexArray();
    ~VertexArray();

    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

    void SetPrimativeType(const unsigned int type_in);
    unsigned int GetPrimativeType(void) const;

    unsigned int GetVertexArray(void) const;

    void Bind(void) const;
    void Unbind(void) const;
};

