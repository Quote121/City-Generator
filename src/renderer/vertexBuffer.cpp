#include <vertexBuffer.hpp>
#include <glad/glad.h>

// Explicit template instantation
template void VertexBuffer::SetData<float>(const void* data, const unsigned int size);
template void VertexBuffer::SetData<unsigned int>(const void* data, const unsigned int size);
template void VertexBuffer::SetData<double>(const void* data, const unsigned int size);


VertexBuffer::VertexBuffer(void)
{
    // If 0 (not initalized, we generate a value)
    if (!VBO) glGenBuffers(1, &VBO);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &VBO);
}

template<typename T>
void VertexBuffer::SetData(const void* data, const unsigned int size)
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(T), data, GL_STATIC_DRAW);
}

void VertexBuffer::Bind(void) const
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
}

void VertexBuffer::Unbind(void) const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

