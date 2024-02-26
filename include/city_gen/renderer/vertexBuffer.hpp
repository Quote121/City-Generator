#pragma once


class VertexBuffer
{
private:
    unsigned int VBO = 0;
public:

    VertexBuffer(void);
    ~VertexBuffer(void);

    // @param size - number of elements
    template<typename T>
    void SetData(const void* data, const unsigned int size);

    template<typename T>
    void CreateBuffer(const unsigned int size);

    void CreateBuffer(const unsigned int bytes);

    void UpdateBuffer(const void* data, const unsigned int offset, const unsigned int size);

    void Bind(void) const;
    void Unbind(void) const;
};

