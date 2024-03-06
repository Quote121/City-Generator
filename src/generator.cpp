#include "config.hpp"
#include "glm/exponential.hpp"
#include "road_object.hpp"
#include <algorithm>
#include <generator.hpp>
#include <map>
#include <sstream>    
#include <stack>

#include <stopwatch.hpp>

// Macro for logging
#define LOG_GEN "GENERATOR"

// Helper function
// Checks if the distance between two points is less than a threshold value in the XZ plane
// Road length has to scale with the threshold value
inline bool inRangeXZPlane(const road_gen_point& a, const road_gen_point& b, const float threshold, const float roadWidth, const float roadLength){
    float distance = glm::sqrt(glm::pow(a.point.x - b.point.x, 2.0f) + glm::pow(a.point.z - b.point.z, 2.0f));
    // If close enough and as far away as road width
    if (distance > threshold*roadLength || distance < roadWidth){
        return false;
    }    
    return true;
}


void generator::GenerateRoads(int iterations = 2, float roadLength = 10.0f, float roadWidth = 3.0f, float roadAngleDegrees = 90.0f)
{
    auto roadGenerateStartTime = StopWatch::GetCurrentTimePoint();

    // Warning as this will cause z-fighting
    if (roadWidth >= roadLength)
    {
        LOG(WARN_SERV(LOG_GEN), "RoadLength is less than or equal to roadWidth. This can cause Z-fighting.")
    }

    LOG(STATUS, "[ Starting GenerateRoads ]");

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


    // Output for current settings for the road generation
    LOG(STATUS, "==== Generation settings ====");
    LOG(STATUS, "||\tIterations:\t" << iterations);
    LOG(STATUS, "||\tRoadLength:\t" << roadLength);
    LOG(STATUS, "||\tRoadWidth: \t" << roadWidth);
    LOG(STATUS, "||\tRoadAngle: \t" << roadAngleDegrees << " (degrees)");
    LOG(STATUS, "==== Generation settings ====");

    // grammar
    std::string treeGrammar = "X"; // Needs to be changed based on what our grammar is
    generator::LSystemGen(&treeGrammar, iterations); // Get 2 iterations on the grammar

    std::stack<road_gen_point> pointStack;

    // End nodes from roads that are delted later will still be here, not an issue for the final product
    std::vector<road_gen_point> endPoints;

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
            
            roadsVector.push_back({currentPoint.point, nextPoint, roadWidth}); // ^^

            // If the end point is in the vector already we wont add.
            // This is done instead of a set as later on we need to access the values and attributes and dont want to const cast
            auto iter = std::find_if(endPoints.begin(), endPoints.end(), [&](const road_gen_point& point)
            {
                return ((point.point == nextPoint) && (point.degreeHeading == currentPoint.degreeHeading));
            });
            
            if (iter == endPoints.end())
            {
                endPoints.push_back({nextPoint, currentPoint.degreeHeading});
            }

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
            LOG(WARN, "Default taken on switch in GenerateRoads()")
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
    LOG(STATUS, "[" << removedRoadsDupes << "] roads removed due to duplicates.");
    

    // For all the remaining roads we now have to calculate their line properties to determine if they intersect
    for (auto& road : roadsVector)
    {
        road.UpdateLineProps();
    }

    
    unsigned int removedRoadsIntersect = roadsVector.size(); 
    for (auto i_it = roadsVector.begin(); i_it != roadsVector.end();)
    {
        for (auto j_it = roadsVector.begin(); j_it != roadsVector.end();)
        {
            // if (i_it->isIntercepting(*j_it))
            if (i_it->isInterceptingAndNodes(*j_it))
            // if (i_it->isIntercepting(*j_it))
            {
                // If we remove at j then we erase and pass the next valid object back to the iterator
                removedRoadsIntersect++;
                j_it = roadsVector.erase(j_it);
            }
            else 
            {
                // Otherwise we move onto the next one
                ++j_it;
            }
        }
        ++i_it;
    }
    removedRoadsIntersect -= roadsVector.size(); 
    LOG(STATUS, "[" << removedRoadsIntersect << "] roads removed due to intersections.");

    // Connect nodes that are allowed to be connected (if they are close enough and do not intersect anything)
    float connectionThresholdDistance = 5.0f;
    
   
    // This creates new roads based on the end node list generated earlier.
    // The criteria for a new end node road are the following
    // - A and B are not the same node
    // - Neither A or B are in use by another end node
    // - The two points A and B are in range in the xz plane as specified by a threshold value
    // - The road between A and B does not intersect any other existing road.
    // If all of the above are satisfied then a road is generated.
    unsigned int endNodeConnections = 0;
    for(auto i_it = endPoints.begin(); i_it != endPoints.end();)
    {
    
        for (auto j_it = endPoints.begin(); j_it != endPoints.end();)
        {
        
            // Not looking at the same nodes and they have not already been used
            if (i_it != j_it && !i_it->endNodeUsed && !j_it->endNodeUsed)
            {
                
                if (inRangeXZPlane(*i_it, *j_it, connectionThresholdDistance, roadWidth, roadLength))
                {
                    // Create temporary road
                    road_gen_road tempRoad = {i_it->point, j_it->point, roadWidth};
                    bool endNodeRoadIntersection = false; // used to exit deeply nested loops
                    for (unsigned int i = 0; i < roadsVector.size(); i++)
                    {
                        // If intersects then we look at next point
                        if (tempRoad.isInterceptingAndNodes(roadsVector[i]))
                        {
                            endNodeRoadIntersection = true;
                            break;
                        }
                    }
                    // If no intersections we add directy to endPoints
                    if (!endNodeRoadIntersection)
                    {
                        // Add to vector
                        roadsVector.push_back(tempRoad);
                        // Set nodes to used
                        i_it->endNodeUsed = true; j_it->endNodeUsed = true;
                        endNodeConnections++;
                    }
                }
            }
            ++j_it;
        }
        ++i_it;
    }
    
    LOG(STATUS, "[" << endNodeConnections << "] new roads created from end node connections.");

    // Then we add to the scene for rendering
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
    LOG(STATUS, "[ GenerateRoads finished. Time elapsed: " << timeElapsed << "ms ]\n");

    // Update the batch renderer buffers
    Scene::getInstance()->roadBatchRenderer->UpdateAll();

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
            // {{"X", "F[+F[+X]F[-X]]"}};

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

void generator::CalculateValidZones()
{
    
    LOG(STATUS, "[ Started GenerateValidZones ]");
    auto generateValidZonesStartTime = StopWatch::GetCurrentTimePoint();

    // Get all roads in the scene and determine the zones either side of the roads
    std::vector<RoadObject*> sceneRoads = Scene::getInstance()->GetRoadObjects();
    

    bool zoneACollide = false;
    bool zoneBCollide = false;

    unsigned int collisionZoneCount = 0;
    for (size_t i = 0; i < sceneRoads.size(); i++)
    {
        zoneACollide = false; zoneBCollide = false;
        for (size_t j = 0; j < sceneRoads.size(); j++)
        {
            // Optimization cull those which are too far away to be considered
            if (i != j && !sceneRoads[i]->TooFarForCollision(sceneRoads[j], 1.0f))
            {
                // Boolean for both left and right

                if (sceneRoads[i]->GetZoneA()->Intersects(sceneRoads[j]->GetRoadOBB()))
                {
                    sceneRoads[i]->GetZoneA()->SetZoneUsable(false);
                    collisionZoneCount++;
                    zoneACollide = true;
                }
                if (sceneRoads[i]->GetZoneB()->Intersects(sceneRoads[j]->GetRoadOBB()))
                {
                    sceneRoads[i]->GetZoneB()->SetZoneUsable(false);
                    collisionZoneCount++;
                    zoneBCollide = true;
                }
                // If both already collide we can skip onto the next i road
                if (zoneACollide && zoneBCollide)
                {
                    break;
                }
            }
        }
    }

    float percent = static_cast<float>(collisionZoneCount)/(sceneRoads.size()*2)*100;

    LOG(STATUS, "Zones that collided: " << collisionZoneCount << "/" << sceneRoads.size()*2 << " (" << percent << "%)");

    uint64_t timeElapsed = StopWatch::GetTimeElapsed(generateValidZonesStartTime);
    LOG(STATUS, "[ GenerateAssets finished. Time elapsed: " << timeElapsed << "ms ]\n");
}

// Reset colour back to green
void generator::ClearZoneCollisions()
{
    std::vector<RoadObject*> roads = Scene::getInstance()->GetRoadObjects();

    for (auto& road : roads)
    {
        road->GetZoneA()->SetColour(GREEN);
        road->GetZoneB()->SetColour(GREEN);
    }
}

#define MAXLOOPS 20

void generator::GenerateBuildings()
{
    LOG(STATUS, "[ Started GeneratedBuildings ]");
    auto buildingGenerateStartTime = StopWatch::GetCurrentTimePoint();

    // TEMP
    std::string buildingTest = "../assets/models/Buildings/buildingTest2.obj";
    ShaderPath buildingShader = {paths::building_defaultVertShaderPath, paths::building_defaultFragShaderPath}; 


    Scene* scene = Scene::getInstance();

    std::vector<RoadObject*> roads = scene->GetRoadObjects();

    int buildingCount = 0;

    std::vector<PlacementArea> areas;

    for (auto& road : roads)
    {
        for (int i = 0; i < MAXLOOPS; i++)
        {
            auto zone = road->GetZoneA()->GetValidPlacement();

            // Check we still have zones left
            if (zone != nullptr && road->GetZoneA()->IsUsable())
            {
                areas.push_back(*zone);
                buildingCount++;
            }
            else 
                break;
        }       
    }

    for (auto& road : roads)
    {
        for (int i = 0; i < MAXLOOPS; i++)
        {
            auto zone = road->GetZoneB()->GetValidPlacement();

            // Check we still have zones left
            if (zone != nullptr && road->GetZoneB()->IsUsable())
            {
                areas.push_back(*zone);
                buildingCount++;
            }
            else 
                break;
        }       
    }
    LOG(STATUS, "[" << buildingCount << "] Buildings generated."); 
 
    // Pass to remove overlapping buildings
    int intersectingBuildings = 0;
    for (int i = 0; i < areas.size(); i++)
    {
        bool intersects = false;
        for (int j = i+1; j < areas.size(); j++)
        {
            if (areas[i].Intersects(areas[j].zoneVerticesArray))
            {
                intersects = true;
                break;
            }
        }
        if (!intersects)
        {
            scene->addModel(buildingTest, &buildingShader)
                ->SetOriginFrontLeft()
                ->SetPosition(areas[i].position)
                ->ShowBoundingBox(false)
                ->SetRotation(glm::vec3{0, areas[i].angle, 0})
                ->SetInstaceRendering(true);
        }
        else {
            intersectingBuildings++;
        }
    }
    LOG(STATUS, "[" << intersectingBuildings << "] buildings removed. (" << static_cast<float>(intersectingBuildings)/static_cast<float>(buildingCount) * 100 << "%)");

    uint64_t timeElapsed = StopWatch::GetTimeElapsed(buildingGenerateStartTime);
    LOG(STATUS, "[ GenerateBuildings finished. Time elapsed: " << timeElapsed << "ms ]\n");

}

