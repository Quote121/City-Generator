#include <road.hpp>
#include <glad/glad.h>
#include <config.hpp>
#include <shader.hpp>
#include <resourceManager.hpp>

#include <vector>

Road::Road(Shader* shader)
{
    roadShader = shader;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
}

Road::~Road()
{

    glDeleteBuffers(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

// Helper function for getting cross products and normalizing
glm::vec3 inline getCross(glm::vec3 a, glm::vec3 b)
{
    return glm::normalize(glm::cross(a, b));
}

void Road::UpdateVertices(glm::vec3 point_a, glm::vec3 point_b)
{
    int numberOfSides = roadCurveSides;

    const float radius = width/2;
    float theta = (2*M_PI)/roadCurveSides; // radians

    // To add to offset of circle angle
    const float lineAngle = glm::atan((point_b.z - point_a.z)/(point_b.x - point_a.x));
    // const float lineAngle = glm::atan((point_a.z - point_b.z)/(point_a.x - point_b.x));




    // Flip if we switch sides, this will flip the direction that the semi-circles will be facing
    int flip = 1;
    if (point_a.x <= point_b.x) { flip = -1; }

    // Normals of the semi-circles are all the same and are straight up
    // When we get to the slope thats when we need to add normal calculations to determine it

    std::vector<float> verts = {};

    // First side A
    verts.insert(verts.end(), {point_a.x, point_a.y, point_a.z});
    verts.insert(verts.end(), {0.0f, 1.0f, 0.0f});

    // Semi circle just 1/4 to 3/4 = [(sides/4)theta, (3*sides / 4)theta]
    for (int i = numberOfSides/4 ; i < static_cast<float>(numberOfSides) * (3.0f/4.0f) + 1; i++)
    {   
        // Add PI to flip 180
        float xOffset = radius * (glm::cos((i * theta) + lineAngle + M_PI)) * flip;
        float zOffset = radius * (glm::sin((i * theta) + lineAngle + M_PI)) * flip;
        verts.insert(verts.end(), {point_a.x + xOffset, point_a.y, point_a.z + zOffset});

        // Normal up
        verts.insert(verts.end(), {0.0f, 1.0f, 0.0f});
    }
    
    // B circle
    verts.insert(verts.end(), {point_b.x, point_b.y, point_b.z});
    verts.insert(verts.end(), {0.0f, 1.0f, 0.0f});

    
    for (int i = numberOfSides/4 ; i < static_cast<float>(numberOfSides) * (3.0f/4.0f) + 1; i++)
    {
        float xOffset = radius * (glm::cos((i * theta) + lineAngle)) * flip;
        float zOffset = radius * (glm::sin((i * theta) + lineAngle)) * flip;
        verts.insert(verts.end(), {point_b.x + xOffset, point_b.y, point_b.z + zOffset});

        // Normal up
        verts.insert(verts.end(), {0.0f, 1.0f, 0.0f});
    }

    /*
    //
    // This is the layout of the road where each number is a vertex and A B are the two points
    //
    //  __ 1_3_________________5_7 __
    // /  |  |                 |  |  \ 
    // | A|  |                 |  |B |
    // \__|__|_________________|__|__/
    //     2 4                 6 8
    //
    // The verts.insert() are each labeled by a number
    //
    ////////////////////////////
    // Draw 3 boxes for the road
    ////////////////////////////
    */
    // dont include Y component into unit vector as it causes the road to thin when normalizing
    glm::vec3 unitVecAB = glm::normalize(glm::vec3{point_b.x, 0, point_b.z} - glm::vec3{point_a.x, 0, point_a.z});
    glm::vec3 invUnitVecAB = glm::vec3{-unitVecAB.z, unitVecAB.y, unitVecAB.x};
    
    // All 4 verts of point A
    glm::vec3 leftALine = point_a + (invUnitVecAB * radius);
    glm::vec3 rightALine = point_a - (invUnitVecAB * radius);
    
    glm::vec3 point_a_offset = {point_a.x + (radius * unitVecAB.x), point_a.y, point_a.z + (radius * unitVecAB.z)};
    glm::vec3 leftA = point_a_offset + (invUnitVecAB * radius);
    glm::vec3 rightA = point_a_offset - (invUnitVecAB * radius);

    // All 4 verts of point B
    glm::vec3 leftBLine = point_b + (invUnitVecAB * radius);                    
    glm::vec3 rightBLine = point_b - (invUnitVecAB * radius);
    
    glm::vec3 point_b_offset = {point_b.x - (radius * unitVecAB.x), point_b.y, point_b.z - (radius * unitVecAB.z)};
    glm::vec3 leftB = point_b_offset + (invUnitVecAB * radius);
    glm::vec3 rightB = point_b_offset - (invUnitVecAB * radius);

    // 4 and 5
    glm::vec3 threeNorm = getCross(glm::vec3{rightA.x, point_a.y, rightA.z}, glm::vec3{leftB.x, point_b.y, leftB.z});
    // 3 and 6
    glm::vec3 fourNorm = getCross(glm::vec3{leftA.x, point_a.y, leftA.z}, glm::vec3{rightB.x, point_b.y, rightB.z});
    LOG(STATUS, "ThreeNorm: [" << threeNorm.x << "," << threeNorm.y << "," << threeNorm.z << "]")
    LOG(STATUS, "FourNorm: [" << fourNorm.x << "," << fourNorm.y << "," << fourNorm.z << "]")


    // Adjacent to point A
    verts.insert(verts.end(), {leftALine.x, point_a.y, leftALine.z});           // 1
    verts.insert(verts.end(), {0.0f, 1.0f, 0.0f});

    verts.insert(verts.end(), {rightALine.x, point_a.y, rightALine.z});         // 2
    verts.insert(verts.end(), {0.0f, 1.0f, 0.0f});

    // Left and right
    verts.insert(verts.end(), {leftA.x, point_a.y, leftA.z});                   // 3
    verts.insert(verts.end(), {threeNorm.x, threeNorm.y, threeNorm.z});

    verts.insert(verts.end(), {rightA.x, point_a.y, rightA.z});                 // 4
    verts.insert(verts.end(), {fourNorm.x, fourNorm.y, fourNorm.z});


    // Left and right
    verts.insert(verts.end(), {leftB.x, point_b.y, leftB.z});                   // 5
    verts.insert(verts.end(), {fourNorm.x, fourNorm.y, fourNorm.z});

    verts.insert(verts.end(), {rightB.x, point_b.y, rightB.z});                 // 6
    verts.insert(verts.end(), {threeNorm.x, threeNorm.y, threeNorm.z});


    // Adjacent to point B
    verts.insert(verts.end(), {leftBLine.x, point_b.y, leftBLine.z});           // 7
    verts.insert(verts.end(), {0.0f, 1.0f, 0.0f});

    verts.insert(verts.end(), {rightBLine.x, point_b.y, rightBLine.z});         // 8
    verts.insert(verts.end(), {0.0f, 1.0f, 0.0f});
    //========================//


    // Needed for draw
    unsigned int roadVertices = verts.size(); 

    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, roadVertices * sizeof(float), verts.data(), GL_STATIC_DRAW);

    // APos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // aNormals offset by 3 floats for each vert
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0); // Unbind

}

void Road::Draw(glm::mat4 view, glm::mat4 projection)
{


    // glLineWidth(3.0f);
    glBindVertexArray(VAO);

    // Draw both semi-circles
    // GL_TRIANGLE_FAN draws n-2 for n
    glDrawArrays(GL_TRIANGLE_FAN, 0, roadCurveSides/2 + 2); // Strips
    glDrawArrays(GL_TRIANGLE_FAN, roadCurveSides/2 + 2, roadCurveSides/2 + 2); // Strips 2

    // Draw the 3 box parts of the road
    // n-2 so we need 8 verts for 6 triangles
    glDrawArrays(GL_TRIANGLE_STRIP, roadCurveSides + 4 , 8);

    glBindVertexArray(0);

    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        LOG(ERROR, "OpenGL Line::Draw() Error: " << error);
    }
}
