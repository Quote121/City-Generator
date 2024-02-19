#include <vertexBufferLayout.hpp>
#include <glad/glad.h>
#include <cassert>

// Inspired from the Cherno : https://www.youtube.com/@TheCherno
unsigned int VertexBufferElement::GetSizeOfType(unsigned int type)
{
    switch(type)
    {
        case GL_FLOAT         : return sizeof(GLfloat);
        case GL_UNSIGNED_INT  : return sizeof(GLuint);
        case GL_UNSIGNED_BYTE : return sizeof(GLbyte);
    }
    assert(false); // A type that doesn't exist has been passed
    return 0;
}


void VertexBufferLayout::Push(unsigned int type, unsigned int count, unsigned char normalized)
{
    elementVector.push_back({type, count, normalized});
    elementVector.push_back({type, count, normalized});
    stride += count * VertexBufferElement::GetSizeOfType(type);
}

VertexBufferLayout::VertexBufferLayout() : stride() {}

void VertexBufferLayout::AddFloat(unsigned int count)        { Push(GL_FLOAT, count, GL_FALSE);        }
void VertexBufferLayout::AddUnsignedInt(unsigned int count)  { Push(GL_UNSIGNED_INT, count, GL_FALSE); }
void VertexBufferLayout::AddUnsignedByte(unsigned int count) { Push(GL_UNSIGNED_BYTE, count, GL_TRUE); }

const std::vector<VertexBufferElement> VertexBufferLayout::GetElements() const 
{
    return elementVector;
}

unsigned int VertexBufferLayout::GetStride() const 
{
    return stride;
}

