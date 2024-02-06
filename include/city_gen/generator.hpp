#pragma once

#include <scene.hpp>
#include <config.hpp>

// Perlin noise generator
#include <Reputeless/PerlinNoise.hpp>
#include <string>

#include <limits> // For float inf

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
        // If dx == 0 then we have a vertical line
        if (dx == 0)
        {
            lp.xzGradient = std::numeric_limits<float>::infinity();
            // No zIntercept
        }
        else
        {
            lp.xzGradient = dz/dx;
            lp.zIntercept = a.z - (lp.xzGradient*a.x);
        }
    }
    
    // To take into tolerance
    inline bool areAboutEqual(float a, float b)
    {
        float tolerance = 0.0001; // Need to have float comparison matching of 0.0001
        return fabs(a-b) < tolerance;
    }

    // Determines if this line and the passed in line(road) intersect
    bool isIntercepting(const road_gen_road& road)
    {
        // Taken from this guy
        // https://flassari.is/2008/11/line-line-intersection-in-cplusplus/

        float x1 = a.x, x2 = b.x, x3 = road.a.x, x4 = road.b.x;
        float y1 = a.z, y2 = b.z, y3 = road.a.z, y4 = road.b.z;

        // Some matrix determinant that indicates how parallel
        float d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
        if (d == 0) return false; // If determinant is zero they are parallel.

        // Get the x and y
        float pre = (x1*y2 - y1*x2), post = (x3*y4 - y3*x4);
        float x = ( pre * (x3 - x4) - (x1 - x2) * post ) / d;
        float y = ( pre * (y3 - y4) - (y1 - y2) * post ) / d;
         
        // Check if the x and y coordinates are within both lines
        if ( x < fmin(x1, x2) || x > fmax(x1, x2) ||
        x < fmin(x3, x4) || x > fmax(x3, x4) ) return false;
        if ( y < fmin(y1, y2) || y > fmax(y1, y2) ||
        y < fmin(y3, y4) || y > fmax(y3, y4) ) return false;

        // Now we check that the point of intersection is not at one of the points
        if ((areAboutEqual(x1, x) && areAboutEqual(y1, y)) ||
            (areAboutEqual(x2, x) && areAboutEqual(y2, y)) ||
            (areAboutEqual(x3, x) && areAboutEqual(y3, y)) ||
            (areAboutEqual(x4, x) && areAboutEqual(y4, y)))
        {
            return false;       
        }
        // If all of that is passed then we know they intersect
        return true;
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

