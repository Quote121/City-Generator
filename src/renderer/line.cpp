#include <line.hpp>

#include <shader.hpp>
// #include <glad/glad.h>

// TODO put all of these in renderer and so then will not need to call again
// #include <vertexArray.hpp>
// #include <vertexBuffer.hpp>
// #include <indexBuffer.hpp>
#include <renderer.hpp>
// #include <vertexBufferLayout.hpp>

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

    VBO->SetData(vertices, 6 * sizeof(float));

    // Set attributes
   
    // Dereferance the vertex buffer to pass
    VAO->AddBuffer(*VBO, layout);

    VAO->Bind();
    VBO->Bind();

    // Deal with index buffer too

    

    


    // OLD OPENGL code
    // glBindVertexArray(VAO);
    // 
    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), vertices, GL_STATIC_DRAW);
    //
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);
}

void Line::Draw()
{
    Renderer::GetInstance()->DrawArrays(*VAO, 2);
    
    //
    // // glLineWidth(2.0f);
    // glBindVertexArray(VAO);
    // glDrawArrays(GL_LINES, 0, 2);
    // glBindVertexArray(0);
    //
    // GLenum error;
    // while ((error = glGetError()) != GL_NO_ERROR) {
    //     LOG(ERROR, "OpenGL Line::Draw() Error: " << error);
    // }
}

