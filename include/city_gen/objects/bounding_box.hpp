#pragma once
/*
    This is the header for the bounding box which the base_object has
    The bounding box is a AABB which takes each coord and determines the box

    Here the bounding box is not placed in the renderer as it is more to do with objects
    However it does contain openGL calls. Since its quite self contained I dont feel the need
    to add it to the renderer like with the other. Broke the unspoken rule of all opengl
    in /renderer, ah well. Im sure I wont go to hell for it... right?
*/


#include <glm/glm.hpp>
#include "config.hpp"

class BoundingBox
{
private:
    // All measurements are in local coordinate space
    // Min xyz
    glm::vec3 min;
    // Max xyz
    glm::vec3 max;

    unsigned int VAO, VBO;

    Shader* boundingBoxShader;

public:

    // Defines the VAO and VBO with the updated vertices
    void SetupBuffers()
    {
        float vertices[]{
            // Top square
            //  x  y  z
            max[0], max[1], max[2], 
            //  x  y -z
            max[0], max[1], min[2],
            // -x  y -z
            min[0], max[1], min[2],
            // -x  y  z
            min[0], max[1], max[2],

            // Bottom square
            //  x -y  z
            max[0], min[1], max[2],
            //  x -y -z
            max[0], min[1], min[2],
            // -x -y -z
            min[0], min[1], min[2],
            // -x -y  z
            min[0], min[1], max[2],

            // Forward right vertical
            max[0], max[1], max[2], 
            max[0], min[1], max[2],
            // Back right vertical
            max[0], max[1], min[2],
            max[0], min[1], min[2],
            // Back left vertical
            min[0], max[1], min[2],
            min[0], min[1], min[2],
            // Forward left vertical
            min[0], max[1], max[2],
            min[0], min[1], max[2]
        };

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        
        glBufferData(GL_ARRAY_BUFFER, 48 * sizeof(float), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0); // Unbind
    }


    BoundingBox()
    {
        // Set min and max to the greatest and smallest values respectivley upon initalization
        min = glm::vec3(std::numeric_limits<float>::max());
        max = glm::vec3(-std::numeric_limits<float>::max());

        boundingBoxShader = new Shader(paths::bb_defaultVertShaderPath, paths::bb_defaultFragShaderPath);
        
        // Setup VAO and VBOs
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        // SetupBuffers();
    }

    ~BoundingBox()
    {
        delete(boundingBoxShader);
        glDeleteBuffers(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    // When creating the object we use this method
    void StreamVertexUpdate(glm::vec3& point)
    {
        min = glm::min(min, point);
        max = glm::max(max, point);
    }

    // When we update the bounding box after creation we use this method
    void Update(glm::vec3& point)
    {
        min = glm::min(min, point);
        max = glm::max(max, point);

        // Since we need to update the vertices in the buffer objects
        SetupBuffers();
    }

    // Getters
    inline glm::vec3 getMin()
    {
        return min;
    }

    inline glm::vec3 getMax()
    {
        return max;
    }

    inline glm::vec3 getCenter()
    {
        return (min + max) * 0.5f;
    }

    inline Shader* getShader()
    {
        return boundingBoxShader;
    }

    // Draw function maybe for a wireframe like draw

    // The setup for the box VAO and VBO will be done everytime the update() is called
    void Draw()
    {
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINE_LOOP, 0, 4);
        glDrawArrays(GL_LINE_LOOP, 4, 4);
        glDrawArrays(GL_LINES, 8, 8);

        glBindVertexArray(0);
    
        GLenum error;
        while ((error = glGetError()) != GL_NO_ERROR) {
            LOG(ERROR, "OpenGL BoundingBox::Draw() Error: " << error);
        }
    }

};
