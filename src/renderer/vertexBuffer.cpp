#include <vertexBuffer.hpp>
#include <glad/glad.h>

VertexBuffer::VertexBuffer(void)
{
    // If 0 (not initalized, we generate a value)
    if (!VBO) glGenBuffers(1, &VBO);
}

VertexBuffer::VertexBuffer(const void* data, const unsigned int size) : VertexBuffer()
{
    SetData(data, size);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &VBO);
}

void VertexBuffer::SetData(const void* data, const unsigned int size)
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void VertexBuffer::Bind(void) const
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
}

void VertexBuffer::Unbind(void) const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

