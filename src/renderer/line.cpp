#include "line.hpp"

Line::Line(Shader* shader)
{
    lineShader = shader;
}

void Line::UpdateVerts(glm::vec3 point_a, glm::vec3 point_b)
{
    float vertices[] = {
        point_a[0], point_a[1], point_a[2],
        point_b[0], point_b[1], point_b[2]
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0); // Unbind
}

void Line::Draw()
{
    LOG(STATUS, "VAO for Line: " << VAO);

    // glLineWidth(2.0f);
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);

    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        LOG(ERROR, "OpenGL Line::Draw() Error: " << error);
    }
}

