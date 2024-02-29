#include <vertexBuffer.hpp>
#include <glad/glad.h>

// Explicit template instantation
template void VertexBuffer::SetData<float>(const void* data, const unsigned int size);
template void VertexBuffer::CreateBuffer<float>(const unsigned int size);

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

// Create buffer of size with no data
template<typename T>
void VertexBuffer::CreateBuffer(const unsigned int size)
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(T), nullptr, GL_STATIC_DRAW);
}

void VertexBuffer::CreateBuffer(const unsigned int bytes)
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, bytes, nullptr, GL_STATIC_DRAW);
}

// Update section of buffer with data
void VertexBuffer::UpdateBuffer(const void* data, const unsigned int offset, const unsigned int size_bytes)
{
    this->Bind();
    glBufferSubData(GL_ARRAY_BUFFER, offset, size_bytes, data); 
}


void VertexBuffer::Bind(void) const
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
}

void VertexBuffer::Unbind(void) const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

