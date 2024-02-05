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

struct LineProperties
{
    float xzGradient;
    float zIntercept;
};

// Temp struct for holding road data before adding it to the scene
struct road_gen_road
{
    glm::vec3 a;
    glm::vec3 b;
    float width;
    glm::vec3 colour = DEFAULT_ROAD_COLOUR;
    LineProperties lp;

    // This gets the stats about the line in the xz plane
    void UpdateLineProps(void)
    {
        // Work out xz gradient
        float dz = b.z - a.z;
        float dx = b.x - a.x;
        lp.xzGradient = dz/dx;

        // Work out zIntercept, subbing a
        lp.zIntercept = a.z - (lp.xzGradient*a.x);
    }

    // Determines if this line and the passed in line(road) intersect
    bool isIntercepting(const road_gen_road& road)
    {
        // we are z = m x + c 
        // theyre z`= m`x + c`
        //
        // subbing them into us and solving for x we get
        //
        // m`x + c` = m x + c
        //  x = (c - c`)/(m`-m)
        
        float x = (lp.zIntercept - road.lp.zIntercept)/(road.lp.xzGradient - lp.xzGradient);
        
        // Then we check if x is in the range to say if its an intersect


        return false;
    }

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

