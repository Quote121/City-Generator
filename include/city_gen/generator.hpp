#pragma once

#include <scene.hpp>
#include <config.hpp>

// Perlin noise generator
#include <Reputeless/PerlinNoise.hpp>
#include <string>

// Struct used for road generation
struct road_gen_point
{
    glm::vec3 point;
    float degreeHeading;
};

// Struct definition for the set
inline bool operator<(const road_gen_point& lhs, const road_gen_point& rhs)
{
    if (lhs.point.x != rhs.point.x)
    {
        return lhs.point.x < rhs.point.x;
    }
    if (lhs.point.y != rhs.point.y)
    {
        return lhs.point.y < rhs.point.y;
    }
    return lhs.point.z < rhs.point.z;
}

namespace generator
{
    void generateRoads(void);

    // Axiom, is the string to have the grammar effect
    // Iterations is amount of iterations on string
    void LSystemGen(std::string* axiom, uint iterations); 

    // Building placement
    void generateBuildings();
        // IDEA
        //
        // For generating the buildings near the roads we must determine where the roads are covering.
        // First way to solve (unpreferred)
        // - Since we have all points and widths of roads we can mathematically determine wether a building overlaps
        //   with a road
        // - We give each road a bounding box, alot like the models we have. Then when we generate the buildings we do
        //   collision detection (O(n^2)), not preffered but there will only be cost upon each generation and not runtime

    // Tree sprite placement
    void generateTrees();
        // IDEA
        //
        // Just like with buildings determine tree placement based on collision
        // We could also define a circle around the generation zone to allow roads to generate
};

