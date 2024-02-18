#include <renderer.hpp>
#include <config.hpp>
#include <glad/glad.h>

Renderer* Renderer::pInstance{nullptr};

Renderer* Renderer::GetInstance()
{
    if (pInstance == nullptr)
    {
        pInstance = new Renderer();
    }
    return pInstance; 
}

void Renderer::DrawIndices(const VertexArray& vao, const IndexBuffer& ebo)
{
    vao.Bind();
    ebo.Bind(); 
    glBindVertexArray(vao.GetVertexArray());
    glDrawElements(GL_TRIANGLES, ebo.GetCount(), GL_UNSIGNED_INT, nullptr); 
}

void Renderer::DrawArrays(const VertexArray& vao, unsigned int count)
{
    // glLineWidth(2.0f);
    glBindVertexArray(vao.GetVertexArray());
    glDrawArrays(vao.GetPrimativeType(), 0, count);

    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        LOG(ERROR, "OpenGL Line::Draw() Error: " << error);
    }

}
