#include <road.hpp>
#include <glad/glad.h>
#include <config.hpp>
#include <shader.hpp>
#include <resourceManager.hpp>

#include <vector>

Road::Road(Shader* shader)
{
    roadShader = ResourceManager::getInstance()->LoadShader(paths::line_defaultVertShaderPath, paths::line_defaultFragShaderPath);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
}

Road::~Road()
{

    glDeleteBuffers(1, &VAO);
    glDeleteBuffers(1, &VBO);
}


// TODO make const
void Road::UpdateVertices(glm::vec3 point_a, glm::vec3 point_b)
{

    // only doing x and z not y
    // glm::vec3 middle = glm::vec3(glm::abs(point_a.x-point_b.x), 0, glm::abs(point_a.z-point_b.z));
    LOG(STATUS, "A: " << point_a.x << " " << point_a.y << " " << point_a.z << " B: "<< point_b.x << " " << point_b.y << " " << point_b.z )

    glm::vec3 norm = glm::normalize(glm::abs(point_a-point_b));
    LOG(STATUS, "NORM: " << norm.x << " " << norm.y << " " << norm.z);

    // Divisions on the line
    // Number has to be odd (will deal with this)
    int divs = 50;
    // Units thick the paths are
    float unitsThick = 0.5;
    
    // divs could be determined 

    glm::vec3 sections = (point_b - point_a); sections /= divs;
    
    std::vector<float> vertices = {};

    /*
        Future Idea we could use GL_triangle fan for the tips of the road
        This would make them smooth rounded and intersecting with one another
        would look more fluid.
    */

    // Main line
    vertices.insert(vertices.end(), {point_a.x, point_a.y, point_a.z});
    

    // Get unit vector then tangent and number
    glm::vec3 unitVecAB = glm::normalize(point_b-point_a);
    

    // Save point_a from modification
    glm::vec3 start = point_a;
    

    glm::vec3 newVec = glm::normalize(glm::vec3{1/unitVecAB.x, unitVecAB.y, 1/unitVecAB.z});
    newVec *= unitsThick;
    
    

    glm::vec3 newSec = start + sections;
    vertices.insert(vertices.end(), {newVec.x + newSec.x , newSec.y, -newVec.z + newSec.z}); // This to box the end off

    for (int i = 0; i < divs-1; i ++)
    {
        // Even
        if (i % 2 == 0)
        {
            vertices.insert(vertices.end(), {-newVec.x + newSec.x , newSec.y, newVec.z + newSec.z});
        }
        // Odd
        else
        {
            vertices.insert(vertices.end(), {newVec.x + newSec.x , newSec.y, -newVec.z + newSec.z});
        }


        // For final addition to box it off
        if (i == divs-2) // -2 becuase we want 1 less before and then for loop is not inclusive
        {
            if (i % 2 == 0)
            {
                vertices.insert(vertices.end(), {newVec.x + newSec.x , newSec.y, -newVec.z + newSec.z});
            }
            else
            {
                vertices.insert(vertices.end(), {-newVec.x + newSec.x , newSec.y, newVec.z + newSec.z});
            }
            LOG(STATUS, "start of end")
        }
        LOG(STATUS, "end")
        newSec = newSec + sections;
    }

    
    
    // END
    vertices.insert(vertices.end(), {point_b.x, point_b.y, point_b.z});

    roadVertices = vertices.size();

    for (int i = 0; i < vertices.size()/3; i++)
    {
        // LOG(STATUS, "Vert [" << i << "] : " << vertices[i*3 + 0] << ", " << vertices[i*3+1] << ", " << vertices[i*3+2]);
    }

    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, roadVertices * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0); // Unbind

    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        LOG(ERROR, "OpenGL Line::update Error: " << error);
    }    

}

void Road::Draw(glm::mat4 view, glm::mat4 projection)
{
    // TEMP
    glm::mat4 result = glm::mat4(1.0f);
    Shader* objectShader = GetRoadShader();

    objectShader->use();
    objectShader->setMat4("view", view);
    objectShader->setMat4("projection", projection);
    objectShader->setMat4("model", result);

    objectShader->setVec3("colour", glm::vec3{1.0f, 0.0f, 0.0f});

    // TEMP
    glLineWidth(50.0f);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, roadVertices/3);
    // glDrawArrays(GL_LINES, 0, roadVertices);

    glBindVertexArray(0);

    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        LOG(ERROR, "OpenGL Line::Draw() Error: " << error);
    }
}