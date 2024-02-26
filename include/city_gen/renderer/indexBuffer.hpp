#pragma once

class IndexBuffer
{
private:
    unsigned int EBO = 0;
    unsigned int indexBufferCount;
public:
    IndexBuffer(void);
    IndexBuffer(const unsigned int* indices, unsigned int count);
    ~IndexBuffer();

    void SetData(const unsigned int* indices, unsigned int count);

    void CreateBuffer(const unsigned int size);
    void UpdateBuffer(const void* data, const unsigned int offset, const unsigned int size);

    unsigned int GetCount(void) const;
    void Bind(void) const;
    void Unbind(void) const;

};
