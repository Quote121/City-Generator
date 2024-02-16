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
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

// Helper function for getting cross products and normalizing
glm::vec3 inline getCross(glm::vec3 origin, glm::vec3 a, glm::vec3 b)
{   
    return glm::normalize(glm::cross(a-origin, b-origin));
}

void Road::UpdateVertices(glm::vec3 point_a, glm::vec3 point_b, float width)
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
    // The naming of variables past this point "LEFT" is -z and "RIGHT" is +z relative to the diagram below
    // This is the layout of the road where each number is a vertex and A B are the two points
    //   -z |      
    //      |       __ 1_3_________________5_7 __
    //      |      /  |  |                 |  |  \ 
    //      |      | A|  |                 |  |B |
    //      |      \__|__|_________________|__|__/
    //      |          2 4                 6 8
    //      |      
    //   +z |___________________________________________                   
    //       -x                                     +x
    //
    // The verts.insert() are each labeled by a number
    //
    // Left hand rule, index finger 1st vector, thumb 2nd vector; middle finger is cross result
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

    glm::vec3 one = {leftALine.x, point_a.y, leftALine.z};
    glm::vec3 two = {rightALine.x, point_a.y, rightALine.z};
    glm::vec3 three = {leftA.x, point_a.y, leftA.z};
    glm::vec3 four = {rightA.x, point_a.y, rightA.z};
    glm::vec3 five = {leftB.x, point_b.y, leftB.z};
    glm::vec3 six = {rightB.x, point_b.y, rightB.z};
    glm::vec3 seven = {leftBLine.x, point_b.y, leftBLine.z};
    glm::vec3 eight = {rightBLine.x, point_b.y, rightBLine.z};

    // Done -'ve in front of normals since the z axis is the other way around
    // 4 5 from point 3
    glm::vec3 threeNormal = -getCross(three, four, five);
    // 6 3 from point 4
    glm::vec3 fourNormal = -getCross(four, six, three);
    // 3 6 from point 5
    glm::vec3 fiveNormal = -getCross(five, three, six);
    // 5 4 from point 6
    glm::vec3 sixNormal = -getCross(six, five, four);
    
    // Adjacent to point A
    verts.insert(verts.end(), {one.x, one.y, one.z});                           // 1
    verts.insert(verts.end(), {0.0f, 1.0f, 0.0f});

    verts.insert(verts.end(), {two.x, two.y, two.z});                           // 2
    verts.insert(verts.end(), {0.0f, 1.0f, 0.0f});

    // Left and right
    verts.insert(verts.end(), {three.x, three.y, three.z});                     // 3
    verts.insert(verts.end(), {threeNormal.x, threeNormal.y, threeNormal.z});

    verts.insert(verts.end(), {four.x, four.y, four.z});                        // 4
    verts.insert(verts.end(), {fourNormal.x, fourNormal.y, fourNormal.z});

    // Left and right
    verts.insert(verts.end(), {five.x, five.y, five.z});                        // 5
    verts.insert(verts.end(), {fiveNormal.x, fiveNormal.y, fiveNormal.z});

    verts.insert(verts.end(), {rightB.x, point_b.y, rightB.z});                 // 6
    verts.insert(verts.end(), {sixNormal.x, sixNormal.y, sixNormal.z});

    // Adjacent to point B
    verts.insert(verts.end(), {seven.x, seven.y, seven.z});                     // 7
    verts.insert(verts.end(), {0.0f, 1.0f, 0.0f});

    verts.insert(verts.end(), {eight.x, eight.y, eight.z});                     // 8
    verts.insert(verts.end(), {0.0f, 1.0f, 0.0f});
    //========================//

    //
    // 4 vertices that fully encapsulate the road on the xz plane
    //
    glm::vec3 point_b_offset_out = {point_b.x + (radius * unitVecAB.x), point_b.y, point_b.z + (radius * unitVecAB.z)};
    glm::vec3 topRightPoint = point_b_offset_out + (invUnitVecAB * radius);
    glm::vec3 bottomRightPoint = point_b_offset_out - (invUnitVecAB * radius);

    glm::vec3 point_a_offset_out = {point_a.x - (radius * unitVecAB.x), point_a.y, point_a.z - (radius * unitVecAB.z)};
    glm::vec3 topLeftPoint = point_a_offset_out + (invUnitVecAB * radius);
    glm::vec3 bottomLeftPoint = point_a_offset_out - (invUnitVecAB * radius);

    road_OBB = {topRightPoint, bottomRightPoint, bottomLeftPoint, topLeftPoint};
    
    // 
    // We need the 8 vertices that will define out left and right zone for the zoning algorithm
    // Radius*2 as we want the area besides to be as big as road, this can be changed later
    //
    // left zone, 3 and 5 used
    glm::vec3 leftZone_topLeft = three + (invUnitVecAB * (radius*2));
    glm::vec3 leftZone_topRight = five + (invUnitVecAB * (radius*2));
    road_left_zone_vertices = {three, five, leftZone_topRight, leftZone_topLeft};

    // Right zone, 4 and 6 used
    glm::vec3 rightZone_bottomLeft = four - (invUnitVecAB * (radius*2));
    glm::vec3 rightZone_bottomRight = six - (invUnitVecAB * (radius*2));
    road_right_zone_vertices = {four, six, rightZone_bottomRight, rightZone_bottomLeft};


    // Needed for draw
    unsigned int roadVertices = verts.size(); 
    // LOG(STATUS, "Number of vertices per road: " << roadVertices)

    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, roadVertices * sizeof(float), verts.data(), GL_STATIC_DRAW);

    // APos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // aNormals offset by 3 floats for each vert
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

}

void Road::Draw()
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

