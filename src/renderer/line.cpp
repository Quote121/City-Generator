#include <line.hpp>
#include <shader.hpp>
#include <renderer.hpp>

Line::Line(Shader* shader)
{
    lineShader = shader;

    VAO = new VertexArray();
    VBO = new VertexBuffer(); 
}

Line::~Line()
{
    delete(lineShader);
    delete(VAO);
    delete(VBO);  
}

void Line::UpdateVerts(glm::vec3 point_a, glm::vec3 point_b)
{
    float vertices[] = {
        point_a[0], point_a[1], point_a[2],
        point_b[0], point_b[1], point_b[2]
    };

    VertexBufferLayout layout;
    layout.AddFloat(3); // 3D vectors

    VBO->SetData<float>(vertices, 6);

    // Set attributes
    VAO->AddBuffer(VBO, &layout);

    VAO->Bind();
    VBO->Bind();
}

void Line::Draw()
{
    Renderer::GetInstance()->DrawArrays(VAO, 2, GL_LINES); 
}

