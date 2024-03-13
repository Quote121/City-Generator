#include <road.hpp>
#include <config.hpp>
#include <shader.hpp>
#include <resourceManager.hpp>
#include <renderer.hpp>

#include <vector>

Road::Road(Shader* shader)
{
    roadShader = shader;

    VAO = new VertexArray();
    VBO = new VertexBuffer();
    EBO = new IndexBuffer();
}

Road::~Road()
{
    delete(VAO);
    delete(VBO);
    delete(EBO);
}

// Helper function for getting cross products and normalizing
glm::vec3 inline getCross(glm::vec3 origin, glm::vec3 a, glm::vec3 b)
{   
    return glm::normalize(glm::cross(a-origin, b-origin));
}

void Road::UpdateVertices(glm::vec3 point_a, glm::vec3 point_b, float width)
{
    const int numberOfSides = roadCurveSides;

    const float radius = width/2;
    float theta = (2*M_PI)/roadCurveSides; // radians

    // To add to offset of circle angle
    const float lineAngle = glm::atan((point_b.z - point_a.z)/(point_b.x - point_a.x));

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
    
    glm::vec3 point_a_offset = {point_a.x + (radius * unitVecAB.x), point_a.y, point_a.z + (radius * unitVecAB.z)};
    glm::vec3 leftA = point_a_offset + (invUnitVecAB * radius);
    glm::vec3 rightA = point_a_offset - (invUnitVecAB * radius);

    glm::vec3 point_b_offset = {point_b.x - (radius * unitVecAB.x), point_b.y, point_b.z - (radius * unitVecAB.z)};
    glm::vec3 leftB = point_b_offset + (invUnitVecAB * radius);
    glm::vec3 rightB = point_b_offset - (invUnitVecAB * radius);

    glm::vec3 three = {leftA.x, point_a.y, leftA.z};
    glm::vec3 four = {rightA.x, point_a.y, rightA.z};
    glm::vec3 five = {leftB.x, point_b.y, leftB.z};
    glm::vec3 six = {rightB.x, point_b.y, rightB.z};
    
    // 1, 2, 7, 8 are all overlapped points and so we reuse them from the semi-circles

    // Done -'ve in front of normals since the z axis is the other way around
    // 4 5 from point 3
    glm::vec3 threeNormal = -getCross(three, four, five);
    // 6 3 from point 4
    glm::vec3 fourNormal = -getCross(four, six, three);
    // 3 6 from point 5
    glm::vec3 fiveNormal = -getCross(five, three, six);
    // 5 4 from point 6
    glm::vec3 sixNormal = -getCross(six, five, four);

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

    //
    // 4 vertices that fully encapsulate the road on the xz plane
    //
    glm::vec3 point_b_offset_out = {point_b.x + (radius * unitVecAB.x), point_b.y, point_b.z + (radius * unitVecAB.z)};
    glm::vec3 topRightPoint = point_b_offset_out + (invUnitVecAB * radius);
    glm::vec3 bottomRightPoint = point_b_offset_out - (invUnitVecAB * radius);

    glm::vec3 point_a_offset_out = {point_a.x - (radius * unitVecAB.x), point_a.y, point_a.z - (radius * unitVecAB.z)};
    glm::vec3 topLeftPoint = point_a_offset_out + (invUnitVecAB * radius);
    glm::vec3 bottomLeftPoint = point_a_offset_out - (invUnitVecAB * radius);

    this->road_OBB = {topRightPoint, bottomRightPoint, bottomLeftPoint, topLeftPoint};
    
    this->road_obb_min = {INFINITY, INFINITY, INFINITY};
    this->road_obb_max = {-INFINITY, -INFINITY, -INFINITY};
    // Get road OBB min and max for bounding box testing
    for (int i = 0; i < 4; i++)
    {
        if (road_OBB[i].x < road_obb_min.x) road_obb_min.x = road_OBB[i].x; 
        if (road_OBB[i].y < road_obb_min.y) road_obb_min.y = road_OBB[i].y; 
        if (road_OBB[i].z < road_obb_min.z) road_obb_min.z = road_OBB[i].z;

        if (road_OBB[i].x > road_obb_max.x) road_obb_max.x = road_OBB[i].x;
        if (road_OBB[i].y > road_obb_max.y) road_obb_max.y = road_OBB[i].y;
        if (road_OBB[i].z > road_obb_max.z) road_obb_max.z = road_OBB[i].z;
    }
    
    // 
    // We need the 8 vertices that will define out left and right zone for the zoning algorithm
    // Radius*2 as we want the area besides to be as big as road, this can be changed later
    //
    // left zone, 3 and 5 used
    glm::vec3 leftZone_topLeft = three + (invUnitVecAB * (radius*2));
    glm::vec3 leftZone_topRight = five + (invUnitVecAB * (radius*2));
    this->road_left_zone_vertices = {three, five, leftZone_topRight, leftZone_topLeft};

    // Right zone, 4 and 6 used
    glm::vec3 rightZone_bottomLeft = four - (invUnitVecAB * (radius*2));
    glm::vec3 rightZone_bottomRight = six - (invUnitVecAB * (radius*2));
    this->road_right_zone_vertices = {six, four, rightZone_bottomLeft, rightZone_bottomRight};
    
    std::vector<unsigned int> indices;
    
    unsigned int i = 0;
    // Create index buffer from number of sides info
    for (i = 0; i < static_cast<unsigned int>(numberOfSides/2); i++) // Cast for warn suppression
    {
        // First semi-circle
        indices.insert(indices.end(), {0, i+1, i+2});
    }

    // Next circle starts at numberofsides/2 + 2
    const unsigned int nextCircleStartIndex = numberOfSides/2 + 2;
    for (i = nextCircleStartIndex; i < nextCircleStartIndex + numberOfSides/2; i++)
    {
        indices.insert(indices.end(), {nextCircleStartIndex, i+1, i+2});
    }
  
    const unsigned int one = 1;
    const unsigned int two = 1 + numberOfSides/2;
    const unsigned int sev = 3 + numberOfSides; // Becuase the other semi-circle is drawn like a mirror image
    const unsigned int eig = 3 + numberOfSides/2;

    indices.insert(indices.end(), {one, two, i+2, two, i+2, i+3,    // 123 234
                                   i+2, i+3, i+4, i+3, i+4, i+5,  // 345, 456
                                   i+4, i+5, sev, i+5, sev, eig   // 567, 678
    });

    gVertices = verts;
    gIndices = indices;

    VertexBufferLayout vbl;
    vbl.AddFloat(3); // xyz
    vbl.AddFloat(3); // norms

    VBO->SetData<float>(verts.data(), verts.size());
    VAO->AddBuffer(VBO, &vbl);

    EBO->SetData(indices.data(), indices.size());
}     

void Road::Draw()
{
    // GL_TRIANGLES
    Renderer::GetInstance()->DrawIndices(VAO, EBO);
}

