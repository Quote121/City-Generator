#include <zone.hpp>
#include <glad/glad.h>
#include <config.hpp>

Zone::Zone(const std::array<glm::vec3,4>& array, const float width)
{
    

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // glGenBuffers(1, &EBO);

    UpdateVertices(array, width);
}

Zone::~Zone()
{
    // Delete array objects
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // glDeleteBuffers(1, &EBO);
}

// Method that updates and draws the rectangle for the zone
void Zone::UpdateVertices(const std::array<glm::vec3, 4>& array, const float width)
{
    std::array<float, 12> verts;

    for (unsigned int i = 0; i < array.size(); i++)
    {
        verts[i*3] = array[i].x;
        verts[(i*3)+1] = array[i].y;
        verts[(i*3)+2] = array[i].z;
    }
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);

    // APos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

}

void Zone::Draw()
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    glBindVertexArray(0);

    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        LOG(ERROR, "OpenGL Line::Draw() Error: " << error);
    }
}
