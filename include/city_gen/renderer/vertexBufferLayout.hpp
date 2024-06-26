#pragma once

#include <vector>

// // Inspired from the Cherno : https://www.youtube.com/@TheCherno
class VertexBufferElement
{
public:
    unsigned int type;
    unsigned int count;
    unsigned char normalized; // Bool

    static unsigned int GetSizeOfType(unsigned int type);
};

class VertexBufferLayout
{
private:
    unsigned int stride;

    // Elements such that vertices, normals and texcoords
    std::vector<VertexBufferElement> elementVector;

    void Push(unsigned int type, unsigned int count, unsigned char normalized);

public:
    // Note for future developements, make the Add layout methods take a layout position
    // This will allow more modular shaders as we can set a general layout for them
    VertexBufferLayout();
    void AddFloat(unsigned int count); 
    void AddUnsignedInt(unsigned int count); 
    void AddUnsignedByte(unsigned int count);

    const std::vector<VertexBufferElement> GetElements() const;
    unsigned int GetStride() const;
};

