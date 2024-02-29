#pragma once

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <bounding_box.hpp>
#include <array>

#include <vertexArray.hpp>
#include <vertexBuffer.hpp>
#include <indexBuffer.hpp>

// Forward declaration
class Shader;

class Road
{
private:
   
    VertexArray* VAO;
    VertexBuffer* VBO;
    IndexBuffer* EBO; 

    // as to be multiple of 4, at least 4 (Will createa point at the end of the road at 4)
    // TODO In future this could be set to ranges based on level of detail (close more vertices, further the less)
    unsigned int roadCurveSides = 40; 
    // unsigned int roadCurveSides = 4;//0;     

    Shader* roadShader;

    // Array of the 4 points of the road, all 2d in the xz plane
    std::array<glm::vec3, 4> road_OBB;
    // Array for left zone
    std::array<glm::vec3, 4> road_left_zone_vertices;
    // Array for right zone
    std::array<glm::vec3, 4> road_right_zone_vertices;


    // Road vertices and indices to use for batch renderer
    std::vector<float> gVertices;
    std::vector<unsigned int> gIndices;
    int batchRenderID = -1;

    // Private as we want certain road object values to be updated before
    // Pass both point and road width to recalculate the vertices of the road
    void UpdateVertices(glm::vec3 point_a, glm::vec3 point_b, float width);

public:
    // So that we can access the private values without extra getters for ImGui
    friend class RoadObject;

    // Make the opengl draw calls
    void Draw();
    // void DrawZones();

    Road(Shader* shader);
    ~Road();


    inline std::vector<float> const* GetVertices(void) const
    {
        return &gVertices;
    }

    inline std::vector<unsigned int> const* GetIndices(void) const
    {
        return &gIndices;
    }

    void SetBatchRenderID(const unsigned int id)
    {
        batchRenderID = id;
    }

    unsigned int GetBatchRenderID(void) const
    {
        return batchRenderID;
    }
    

    void SetRoadCurveSides(unsigned int sides)
    {
        if (sides > 0)
        {
            roadCurveSides = sides;
        }
        else
        {
            roadCurveSides = 1;
        }
    }

    inline unsigned int GetRoadCurveSides()
    {
        return roadCurveSides;
    }

    inline Shader* GetRoadShader()
    {
        return roadShader;
    }

    inline std::array<glm::vec3, 4> const& getOBB(void) const
    {
        return road_OBB;
    }

    inline std::array<glm::vec3, 4> const& getLeftZone(void) const
    {
        return road_left_zone_vertices;
    }

    inline std::array<glm::vec3, 4> const& getRightZone(void) const
    {
        return road_right_zone_vertices;
    }

};

