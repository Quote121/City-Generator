#include <indexBuffer.hpp>
#include <glad/glad.h>

IndexBuffer::IndexBuffer()
{
    if (!EBO) glGenBuffers(1, &EBO);
}

IndexBuffer::IndexBuffer(const unsigned int* indices, unsigned int count) : IndexBuffer()
{
    SetData(indices, count);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &EBO);
}

void IndexBuffer::SetData(const unsigned int* indices, unsigned int count)
{
    indexBufferCount = count;
    this->Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
}

// Number of indices
void IndexBuffer::CreateBuffer(const unsigned int size)
{
    this->Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), nullptr, GL_STATIC_DRAW);
}
    
void IndexBuffer::UpdateBuffer(const void* data, const unsigned int offset, const unsigned int size_bytes)
{
    this->Bind();
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size_bytes, data); 
}

unsigned int IndexBuffer::GetCount(void) const
{
    return indexBufferCount;
}

void IndexBuffer::Bind(void) const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
}

void IndexBuffer::Unbind(void) const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

