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
#include <vertexArray.hpp>
#include <vertexBuffer.hpp>
#include <indexBuffer.hpp>

// Forward declaration
class Shader;

class BoundingBox
{
private:
    // All measurements are in local coordinate space
    glm::vec3 min;
    glm::vec3 max;

    VertexArray* VAO;
    VertexBuffer* VBO;
    IndexBuffer* EBO;

    Shader* boundingBoxShader;

public:

    // Defines the VAO and VBO with the updated vertices
    void SetupBuffers();

    BoundingBox();
    ~BoundingBox();

    // When creating the object we use this method
    void StreamVertexUpdate(glm::vec3& point);
    void StreamVertexUpdate(float x, float y, float z);

    // When we update the bounding box after creation we use this method
    void Update(glm::vec3& point);

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

    // For building placement
    inline glm::vec3 getFrontLeftBuilding(void) const
    {
        return {min.x, min.y, min.z};
    }

    inline glm::vec3 getFrontRightBuilding(void) const
    {
        return {max.x, min.y, min.z};
    }

    // The setup for the box VAO and VBO will be done everytime the update() is called
    void Draw();
};

