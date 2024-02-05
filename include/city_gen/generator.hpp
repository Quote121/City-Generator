#pragma once

#include <scene.hpp>
#include <config.hpp>

// Perlin noise generator
#include <Reputeless/PerlinNoise.hpp>
#include <string>

// Struct used for recursive road generation based on point and heading
struct road_gen_point
{
    glm::vec3 point;
    float degreeHeading;

    // Struct definition for the set
    inline bool operator<(const road_gen_point& rhs) const
    {
        if (point.x != rhs.point.x)
        {
            return point.x < rhs.point.x;
        }
        if (point.y != rhs.point.y)
        {
            return point.y < rhs.point.y;
        }
        return point.z < rhs.point.z;
    }
};

// Temp struct for holding road data before adding it to the scene
struct road_gen_road
{
    glm::vec3 a;
    glm::vec3 b;
    float width;

    // Operator overload for comparison
    inline bool operator==(const road_gen_road& roadIn)
    {
        if ((a == roadIn.a) && (b == roadIn.b) && (width == roadIn.width))
        {
            return true;
        }
        return false;
    }

};
namespace generator
{
    // @brief Method for the road generation pass, uses LSystemGen internally to generate a grammar string
    //
    // @args Iterations - number of grammar iterations
    // @args RoadLength - length of the road when generating
    // @args RoadWidth - width of the roads
    // @args roadAngleDegrees - the +/- angles when generating from the grammar
    void generateRoads(int iterations, float roadLength, float roadWidth, float roadAngleDegrees);

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

