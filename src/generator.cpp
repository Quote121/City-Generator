#include "config.hpp"
#include <generator.hpp>
#include <map>
#include <sstream>    
#include <stack>
#include <set>

#include <stopwatch.hpp>


void generator::generateRoads(int iterations = 2, float roadLength = 10.0f, float roadWidth = 3.0f, float roadAngleDegrees = 90.0f)
{
    auto roadGenerateStartTime = StopWatch::GetCurrentTimePoint();

    if (roadWidth >= roadLength)
    {
        LOG(WARN, "generateRoads() roadLength is less than or equal to roadWidth. This can cause Z-fighting.")
    }
    /*
     * General notes so far for the generation
     *
     * At an angle near 90 degrees seem to generate the most realistic looking layouts
     * If we use 160/170 degrees the layout is too dence and does not match with what we would expect from normal roads
     * Based on this we should implement an optional pass that removes roads that overlap.
     * This would also add those to the end nodes list if removed.
     *
     * After that is done another pass to link end nodes to nearby nodes as long as they dont cross an existic road will be done.
     * This will add significant time to the road generation stage but we can take that performance hit as runtime will not be affected.
     *
     * In the terms of runtime, it is evident once we get to a few thousand roads, instancing is a must and should be implemented given enough time.
     * This project is perfect for instancing as we have static meshes that are repeated tonnes of times. Also my awful intergrated graphics cannot
     * deal with this much overhead.
     *
     * TLDR:
     *
     * Add pass to remove roads that already exist at those coordinates
     * Add road intersection checking
     * Add road end node join pass
     * Add instancing
     *
     */


    LOG(STATUS, "generateRoads() started...");
    // std::vector<glm::vec3> points;
    //
    // // TODO, when working we should have a menu handle to tweak all these values and then to generate. That would be cool
    //
    // // Procedural generation of roads here
    // // Seed should be changed based on system time, seed can be saved to replicate the generation
    // const siv::PerlinNoise::seed_type seed = 123456u;
    // const siv::PerlinNoise perlin{ seed };
    //
    // // Note for terrain generation the terrain asset is 160 by 160
    // for (int y = 0; y < 160; ++y)
    // {
    //     for (int x = 0; x < 160; ++x)
    //     {
    //         const double noise = perlin.normalizedOctave2D_01((x), (y), 4);
    //
    //         // Random threshold to create a 
    //         if (noise > 0.65)
    //         {
    //             float xPos, yPos;
    //             xPos = (2*x)-160;
    //             yPos = (2*y)-160;
    //             
    //             // Get Starting points
    //             points.push_back(glm::vec3{xPos, 0, yPos});
    //         }
    //     }
    // }
    //
    // Now we will select several points from the vector of points to begin our L-system and any 
    // nodes close to one another will be joined up

    // grammar
    std::string treeGrammar = "X"; // Needs to be changed based on what our grammar is
    generator::LSystemGen(&treeGrammar, iterations); // Get 2 iterations on the grammar

    std::stack<road_gen_point> pointStack;
    // std::vector<road_gen_point> endPoints; // End nodes of the tree
    std::set<road_gen_point> endPoints; // End nodes of the tree

    // Test point
    glm::vec3 startPoint = {0.0, 0.0, 0.0};
    road_gen_point currentPoint = {startPoint, 0.0f}; // Heading of 0 radians
        
    float degreeRadians = roadAngleDegrees * (M_PI/180);
   

    std::vector<road_gen_road> roadsVector;


    // Key:
    // 
    // F[-X]
    // F Foward n units
    // - Left n degrees
    // + Right n degrees
    // X Forward (end node)
    // [ Push point to stack
    // ] Pop point to stack

    for (char symbol : treeGrammar)
    {
        switch (symbol)
        {
        case 'F':
        {
            // Go forward relative to its angle
            glm::vec3 nextPoint = {
                currentPoint.point.x + (roadLength * glm::sin(currentPoint.degreeHeading)),
                currentPoint.point.y,
                currentPoint.point.z + (roadLength * glm::cos(currentPoint.degreeHeading))
            };

            // Scene::getInstance()->addRoad(currentPoint.point, nextPoint, roadWidth);
            roadsVector.push_back({currentPoint.point, nextPoint, roadWidth}); // Add to our local road vector before adding to the scene
            currentPoint.point = nextPoint; // Update current point, no change to bearing

            break;
        }
        case 'X':
        {
            // Go forward relative to its angle
            glm::vec3 nextPoint = {
                currentPoint.point.x + (roadLength * glm::sin(currentPoint.degreeHeading)),
                currentPoint.point.y,
                currentPoint.point.z + (roadLength * glm::cos(currentPoint.degreeHeading))
            };
            
            // Scene::getInstance()->addRoad(currentPoint.point, nextPoint, roadWidth);
            roadsVector.push_back({currentPoint.point, nextPoint, roadWidth}); // ^^

            endPoints.insert({{nextPoint, currentPoint.degreeHeading}}); // Add to the set of end points
            currentPoint.point = nextPoint; // Update current point, no change to bearing
            
            break;
        }
        case '-':
        {
            currentPoint.degreeHeading +=  degreeRadians;
            // Turn left n degrees
            break;
        }
        case '+':
        {
            currentPoint.degreeHeading -= degreeRadians;
            // Turn right n degrees
            break;
        }
        case '[':
        {
            pointStack.push(currentPoint);
            // Push point to stack
            break;
        }
        case ']':
        {
            currentPoint = pointStack.top(); // Read
            pointStack.pop(); // pop
            // Pop point from stack
            break;
        }
        default:
        {
            LOG(WARN, "Default taken on switch in generateRoads()")
            break;
        }
        } // switch(symbol)
    }

    // Remove duplicate roads
    unsigned int removedRoadsDupes = roadsVector.size();
    // For each road
    for (unsigned int i = 0; i < roadsVector.size(); i++)
    {
        // Start at i+1 (next one)
        for (unsigned int j = i+1; j < roadsVector.size(); j++)
        {
            // If road is in the same remove the one we are looking at (i)
            if (roadsVector[i] == roadsVector[j])
            {
                roadsVector.erase(roadsVector.begin() + i);
            }
        }
    }
    removedRoadsDupes -= roadsVector.size();

    // The numbers will alter based on the length of the roads due to the grammar
    LOG(STATUS, removedRoadsDupes << " roads removed due to duplicates");

    
    // TODO remove intersecting roads, check for each if they are crossing over one another (done during generation)
    // TODO join end nodes up with one another based on if they are close enough and do not cross any other roads.

    // Debug
    // roadsVector.clear();
    // Scene::getInstance()->addRoad({1,0,1}, {1,0,6}, 0.5)->SetColour(BLUE);
    // Scene::getInstance()->addRoad({-4, 0, 3}, {4, 0, 3}, 0.5)->SetColour(BLUE);
    // roadsVector.push_back({{1,0,1}, {1,0,6}, 1.0f});
    // roadsVector.push_back({{-4, 0, 3}, {1, 0, 3}, 1.0f});

    // For all the remaining roads we now have to calculate their line properties to determine if they intersect
    for (auto& road : roadsVector)
    {
        road.UpdateLineProps();
    }

    // unsigned int removedRoadsIntersect = roadsVector.size();
    unsigned int removedRoadsIntersect = roadsVector.size(); 

    for (auto i_it = roadsVector.begin(); i_it != roadsVector.end();)
    {
        for (int i = 0; i < roadsVector.size(); i++)
        {
            if (i_it->isIntercepting(roadsVector[i]))
            {
                removedRoadsIntersect++;
                i_it = roadsVector.erase(i_it);
            }
        }

        ++i_it;
    }

    removedRoadsIntersect -= roadsVector.size(); 
    LOG(STATUS, removedRoadsIntersect << " roads marked due to intersections");

    

    for (int i = 0; i < roadsVector.size(); i++)
    {
        for (int j = 0; j < roadsVector.size(); j++)
        {
            if (roadsVector[i].isIntercepting(roadsVector[j]))
            {
                roadsVector[i].colour = GREEN;
                LOG(STATUS, "Another intersect: i\t" << i << " | j\t" << j)
            }
        }
    }

    
    // Connect nodes that are allowed to be connected (if they are close enough and do not intersect anything)
    float connectionThresholdDistance = 10.0f;







    // Then we add to the scene
    for (auto& road : roadsVector)
    {
        if (road.colour == DEFAULT_ROAD_COLOUR)
        {
            Scene::getInstance()->addRoad(road.a, road.b, road.width);
        }
        else{
            // In hindsight this has minimal overhead but this will only be here temp TODO re-evaluate if this is the best way to go about it
            Scene::getInstance()->addRoad(road.a, road.b, road.width)
                ->SetColour(road.colour);
        }
    }
    
    // Get road number
    size_t numberOfRoads = Scene::getInstance()->GetRoadObjects().size();
    LOG(STATUS, "Number of roads generated: " << numberOfRoads)

    // When generating, we could give a radius from 0,0 for roads to be permitted, this would give a good effect IMO
    uint64_t timeElapsed = StopWatch::GetTimeElapsed(roadGenerateStartTime);
    LOG(STATUS, "generateRoads() finished in " << timeElapsed << "ms");
}

void generator::LSystemGen(std::string *axiom, uint iterations)
{
    if (*axiom == "")
    {
        LOG(WARN, "Axiom is empty, nothing will be generated");
        return;
    }

    for (size_t j = 0; j < iterations; j++)
    {
        std::stringstream ss;
        // Current L system grammar
        std::map<const std::string, const std::string> grammar =
            {{"X", "F[+X]F[-X]F[-X]F[+X]F"},
             {"F", "FF"}}; 
            // {{"F", "F-F++F-F"}};

        for (size_t i = 0; i < axiom->size(); i++)
        {
            // If we have something from the grammar then we need to replace it with the value from the key
            auto value = grammar.find(std::string(1, (*axiom)[i])); // Turn 1 char to string 
            
            if (value != grammar.end())
            {
                ss << value->second;
            }
            // Add the base char value 
            else
            {
                ss << (*axiom)[i];
            }
        }
        // After that all we reasign the axiom* to our new stringstream
        *axiom = ss.str();
    }
}

void generator::generateBuildings();

void generator::generateTrees();


