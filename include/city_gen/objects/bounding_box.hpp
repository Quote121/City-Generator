#pragma once

#include <glm/glm.hpp>


class BoundingBox
{
private:
    // All measurements are in local coordinate space
    // Min xyz
    glm::vec3 min;
    // Max xyz
    glm::vec3 max;

public:
    BoundingBox()
    {
        // Set min and max to the greatest and smallest values respectivley upon initalization
        min = glm::vec3(std::numeric_limits<float>::max());
        max = glm::vec3(-std::numeric_limits<float>::max());
    }

    // Update method
    void Update(glm::vec3& point)
    {
        min = glm::min(min, point);
        max = glm::max(max, point);
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

    // Draw function maybe for a wireframe like draw

    // The setup for the box VAO and VBO will be done everytime the update() is called
    void Draw();

};
