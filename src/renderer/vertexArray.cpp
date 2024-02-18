// #include "vertexBuffer.hpp"
#include <vertexArray.hpp>
#include <vertexBufferLayout.hpp>

#include <glad/glad.h>


VertexArray::VertexArray()
{
    glGenVertexArrays(1, &VAO);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &VAO);
}


void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& vbl) 
{
    // Add each vertexBufferLayout to the vertex array
    Bind();
    vb.Bind();
    const auto elements = vbl.GetElements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        auto element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalized, vbl.GetStride(), reinterpret_cast<void*>(offset));
        offset += element.count + VertexBufferElement::GetSizeOfType(element.type);
    }
}

void VertexArray::SetPrimativeType(const unsigned int type_in)
{
    type = type_in;
}

inline unsigned int VertexArray::GetPrimativeType(void) const
{
    return type;
}

inline GLuint VertexArray::GetVertexArray(void) const
{
    return VAO;
}

inline void VertexArray::Bind(void) const
{
    glBindVertexArray(VAO);
}

inline void VertexArray::Unbind(void) const
{
    glBindVertexArray(0);
}

