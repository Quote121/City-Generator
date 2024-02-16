#pragma once

#include <glad/glad.h>

class VertexArray
{
private:
    
    GLuint array;

public:

    VertexArray();
    ~VertexArray();

    void Bind();

};

