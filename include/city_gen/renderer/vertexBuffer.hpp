#pragma once


class VertexBuffer
{
private:
    unsigned int VBO = 0;
public:

    VertexBuffer(void);
    VertexBuffer(const void* data, const unsigned int size);
    ~VertexBuffer(void);

    void SetData(const void* data, const unsigned int size);
    void Bind(void) const;
    void Unbind(void) const;
};

