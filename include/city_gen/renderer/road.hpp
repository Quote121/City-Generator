#pragma once

#include <glm/glm.hpp>

// Forward declaration
class Shader;

class Road
{
private:
    unsigned int VAO, VBO;
    unsigned int roadVertices;
    Shader* roadShader;
public:

    void UpdateVertices(glm::vec3 point_a, glm::vec3 point_b);

    // Make the opengl draw calls
    void Draw(glm::mat4 view, glm::mat4 projection);

    Road(Shader* shader);
    ~Road();

    inline Shader* GetRoadShader()
    {
        return roadShader;
    }
};
