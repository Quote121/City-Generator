#include <generator.hpp>

#include <config.hpp>
#include <glm/exponential.hpp>
#include <road_object.hpp>
#include <cityRandom.hpp>
#include <stopwatch.hpp>

// STD
#include <algorithm>
#include <map>
#include <sstream>    
#include <stack>

// Macro for logging
#define LOG_GEN "GENERATOR"

// Helper function
// Checks if the distance between two points is less than a threshold value in the XZ plane
// Road length has to scale with the threshold value
inline bool inRangeXZPlane(const road_gen_point& a, const road_gen_point& b, float thresholdUpper, float thresholdLower, const float roadWidth, const float roadLength){
    if (thresholdLower < roadWidth)
        thresholdLower = roadWidth;

    float distance = glm::sqrt(glm::pow(a.point.x - b.point.x, 2.0f) + glm::pow(a.point.z - b.point.z, 2.0f));
    // If close enough and as far away as road width
    if (distance > thresholdUpper*roadLength || distance < thresholdLower){
        return false;
    }    
    return true;
}


struct CityGenerationParameters
{
    glm::vec3 startPosition;
    float startAngle;
    int iterations;         // 2-4
    float roadLength;       //  
    float roadWidth;
    float roadAngleDegrees;
    float densityFactor; // Float that determines the density of a city (likelyhood of a city being built)
};



// @brief Creates long roads between cities
// @args roads - dynamic array of all generated roads, will add the highways to this if any created
// @args endNodes - 2D dynamic array of all the end nodes of each city so we can create highways between different cities
// @args closest - the minimum amount of distance between two end nodes to consider creating a highway
// @args furthest - the furthest amount of distance between two end nodes to conisder creating a highway
void createHighways(std::vector<road_gen_road>* roads, std::vector<std::vector<road_gen_point>>* endNodes, float closest, float furthest, float roadWidth)
{
    // Set our quota of highways to 8, at 8 we stop
    int quota = 1;
    unsigned int newHighways = 0;

    // We cannot create highways between less than 2 cites.
    if (endNodes->size() < 2)
        return;

    // For each set of city end nodes which are not the same city
    for (unsigned int i = 0; i < endNodes->size(); i++)
    {
        for (unsigned int j = i+1; j < endNodes->size(); j++)
        {
            int interCityHighways = 0;
            std::vector<road_gen_point>& cityA = endNodes->at(i);
            std::vector<road_gen_point>& cityB = endNodes->at(j);

            // Attempt to create highways
            for(unsigned int k = 0; k < cityA.size(); k++)
            {
                // Keep the number low for between cities
                if (interCityHighways >= quota)
                    break;

                for (unsigned int l = 0; l < cityB.size(); l++)
                {
                    auto& pointA = cityA[k];
                    auto& pointB = cityB[l];
                    if (inRangeXZPlane(pointA, pointB, furthest, closest, roadWidth, 1.0f) && 
                        !pointA.endNodeUsed && !pointB.endNodeUsed &&
                        !pointA.hasHighway && !pointB.hasHighway)
                    {
                        // Create temporary road
                        road_gen_road tempRoad = {pointA.point, pointB.point, roadWidth*1.5f};
                        tempRoad.allowBuildingZones = false;
                        bool endNodeRoadIntersection = false; // used to exit deeply nested loops
                        for (unsigned int m = 0; m < roads->size(); m++)
                        {
                            // If intersects then we look at next point
                            if (tempRoad.isInterceptingAndNodes(roads->at(m)))
                            {
                                endNodeRoadIntersection = true;
                                break;
                            }
                        }
                        // If no intersections we add directy to endPoints
                        if (!endNodeRoadIntersection)
                        {
                            // Add to vector
                            roads->push_back(tempRoad);
                            newHighways++; interCityHighways++;

                            pointA.hasHighway = true; pointB.hasHighway = true;
                            break;
                        }
                    }
                }
            }
        }
    }
    LOG(STATUS, "[" << newHighways << "] : New highways created.");
}




// @brief removes roads that are the same i.e. overlapping
// @args roadsVector is the vector of roads created originally and is updated by this method
void removeDupes(std::vector<road_gen_road>* roadsVector)
{
    unsigned int removedRoadsDupes = roadsVector->size();
    // For each road

    for (auto i_it = roadsVector->begin(); i_it != roadsVector->end(); i_it++)
    {
        for (auto j_it = i_it+1; j_it != roadsVector->end();)
        {
            // If they are the same, remove i
            if (i_it->a == j_it->a && i_it->b == j_it->b)
            {
                j_it = roadsVector->erase(j_it);
            }
            else {
                j_it++;
            }
        }
    }
    removedRoadsDupes -= roadsVector->size();

    // The numbers will alter based on the length of the roads due to the grammar
    LOG(STATUS, "[" << removedRoadsDupes << "] roads removed due to duplicates.");
}

// @brief Creates new roads from the vector of end nodes
// if a and b are not the same, not used by another node, are close enough and do not intersect other roads
// @args roadsVector Vector of current generated roads
// @args endPoints vector of nodes designated as end nodes and are open for connections
// @args roadWidth 
// @args roadLength
void createNewRoads(std::vector<road_gen_road>* roadsVector, std::vector<road_gen_point>* endPoints, float roadWidth, float roadLength)
{

    // Connect nodes that are allowed to be connected (if they are close enough and do not intersect anything)
    float connectionThresholdDistance = 5.0f;

    unsigned int endNodeConnections = 0;
    for(auto i_it = endPoints->begin(); i_it != endPoints->end(); i_it++)
    {
        for (auto j_it = endPoints->begin(); j_it != endPoints->end(); j_it++)
        {
        
            // Not looking at the same nodes and they have not already been used
            if (i_it != j_it && !i_it->endNodeUsed && !j_it->endNodeUsed)
            {
                
                if (inRangeXZPlane(*i_it, *j_it, connectionThresholdDistance, 0, roadWidth, roadLength))
                {
                    // Create temporary road
                    road_gen_road tempRoad = {i_it->point, j_it->point, roadWidth};
                    bool endNodeRoadIntersection = false; // used to exit deeply nested loops
                    for (unsigned int i = 0; i < roadsVector->size(); i++)
                    {
                        // If intersects then we look at next point
                        if (tempRoad.isInterceptingAndNodes(roadsVector->at(i)))
                        {
                            endNodeRoadIntersection = true;
                            break;
                        }
                    }
                    // If no intersections we add directy to endPoints
                    if (!endNodeRoadIntersection)
                    {
                        // Add to vector
                        roadsVector->push_back(tempRoad);
                        // Set nodes to used
                        i_it->endNodeUsed = true; j_it->endNodeUsed = true;
                        endNodeConnections++;
                    }
                }
            }
        }
    }

    LOG(STATUS, "[" << endNodeConnections << "] new roads created from end node connections.");
}



// Main generation function
int generator::GenerateCity(unsigned int seed_in)
{
    int seed;
    // Generate new city with new random numbers, else use seed
    if (seed_in == 0)
    {
        seed = Random::GenerateSeed();
        std::srand(seed);
    }
    else 
    {
        seed = seed_in;
        std::srand(seed_in);
    }
    
    // First we need to determine how many smaller cities we are going to have
    // 1-4
    int numberOfCities = Random::GetIntBetweenInclusive(1, 6);

    std::vector<CityGenerationParameters> cityParameterVector;
    std::vector<std::vector<road_gen_point>> cityEndNodesVector;
    std::vector<road_gen_road> cityRoads;

    // Then we need to know the parameters to give to each city:
    for (int i = 0; i < numberOfCities; i++)
    {
        cityParameterVector.push_back({
            glm::vec3{Random::GetIntBetweenInclusive(-200, 200), 0, Random::GetIntBetweenInclusive(-200, 200)},
            Random::GetFloatBetweenInclusive(0, 2*M_PI),    // Start angle in radians 
            Random::GetIntBetweenInclusive(2, 4),           // Iterations of grammar
            Random::GetFloatBetweenInclusive(3.0f, 5.0f),   // Road length
            1.0f,                                           // Keep road width the same (1.0f)
            Random::GetFloatBetweenInclusive(88.0f, 92.0f), // Angle between roads in degrees
            Random::GetPercentage()                         // The probability of placing a building at any spot
        });

        // Vector initalizatio
        std::vector<road_gen_point> cityVector;
        cityEndNodesVector.push_back(cityVector);
    }

    LOG(STATUS, "Number of cities: " << numberOfCities);
    for (int i = 0; i < numberOfCities; i++)
    {
        auto city = cityParameterVector[i];
        LOG(STATUS, "\nCITY NUMBER " << i+1);
        LOG(STATUS, "Start vector: " << city.startPosition);
        LOG(STATUS, "Start angle: " << city.startAngle);
        LOG(STATUS, "Iteration: " << city.iterations);
        LOG(STATUS, "Length: " << city.roadLength);
        LOG(STATUS, "Road width: " << city.roadWidth);
        LOG(STATUS, "Road angle: " << city.roadAngleDegrees);
        LOG(STATUS, "Percentage density: " << city.densityFactor);
    }

    
    // TODO another random value passed to the grammar calculator to use several different grammars


    // Valid areas is -200,-200 to 200, 200
    // Thinking out loud:
    //
    // We generate a city netork, it deletes all of its dupes and intersections
    // We then generate a new city network, we delete all dupes and intersection considering all roads in the scene
    // We then create highways from seperate end nodes


    for (size_t i = 0; i < cityParameterVector.size(); i++)
    {
        // Generate and get all end nodes
        // this->GenerateRoads(..., &cityEndNodesVector[i]);
        auto city = cityParameterVector[i];
        auto roads = GenerateRoads(city.startPosition, city.startAngle, city.iterations, city.roadLength, city.roadWidth, city.roadAngleDegrees, &cityEndNodesVector[i], cityRoads);
        cityRoads.insert(cityRoads.end(), roads.begin(), roads.end());
    }

    createHighways(&cityRoads, &cityEndNodesVector, 50.0f, 500.0f, 1.0f);


    for (size_t i = 0; i < cityParameterVector.size(); i++)
    {
        // Create the new roads between end nodes 
        createNewRoads(&cityRoads, &cityEndNodesVector[i], cityParameterVector[i].roadWidth, cityParameterVector[i].roadLength);
    }
    
    // Get road number
    size_t numberOfRoads = Scene::getInstance()->GetRoadObjects().size();
    LOG(STATUS, "Number of roads generated: " << numberOfRoads)

    // Then we add to the scene for rendering
    for (auto& road : cityRoads)
    {
        auto sceneRoad = Scene::getInstance()->addRoad(road.a, road.b, road.width);
        if (!road.allowBuildingZones)
        {
            sceneRoad->GetZoneA()->SetZoneUsable(false);
            sceneRoad->GetZoneB()->SetZoneUsable(false);
        }
    }
    // Update the batch renderer buffers
    Scene::getInstance()->roadBatchRenderer->UpdateAll();

    // LOG(STATUS, "Size of road_gen_road: " << sizeof(road_gen_road)); // 52 bytes last checked

    return seed;

    // TODO random grammars
    //
    // When generating each of our cities we should pick from a list of predetermined grammars
}


std::vector<road_gen_road> generator::GenerateRoads(glm::vec3 startPos,
                                                    float startAngle,
                                                    int iterations,
                                                    float roadLength, 
                                                    float roadWidth, 
                                                    float roadAngleDegrees,
                                                    std::vector<road_gen_point>* endNodes,
                                                    std::vector<road_gen_road>& roadsVector)
{
    auto roadGenerateStartTime = StopWatch::GetCurrentTimePoint();

    // Warning as this will cause z-fighting
    if (roadWidth >= roadLength)
    {
        LOG(WARN_SERV(LOG_GEN), "RoadLength is less than or equal to roadWidth. This can cause Z-fighting.")
    }

    LOG(STATUS, "[ Starting GenerateRoads ]");

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

    // Test point
    // glm::vec3 startPoint = {0.0, 0.0, 0.0};
    road_gen_point currentPoint = {startPos, startAngle}; // Heading of 0 radians
        
    float degreeRadians = roadAngleDegrees * (M_PI/180);
   

    // std::vector<road_gen_road> roadsVector;


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
            auto iter = std::find_if(endNodes->begin(), endNodes->end(), [&](const road_gen_point& point)
            {
                return ((point.point == nextPoint) && (point.degreeHeading == currentPoint.degreeHeading));
            });
            
            if (iter == endNodes->end())
            {
                endNodes->push_back({nextPoint, currentPoint.degreeHeading});
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

    // Pass vector as pointer
    removeDupes(&roadsVector);

    // For all the remaining roads we now have to calculate their line properties to determine if they intersect
    for (auto& road : roadsVector)
    {
        road.UpdateLineProps();
    }

    
    unsigned int removedRoadsIntersect = roadsVector.size(); 
    for (auto i_it = roadsVector.begin(); i_it != roadsVector.end(); i_it++)
    {
        for (auto j_it = roadsVector.begin(); j_it != roadsVector.end();)
        {
            if (i_it->isInterceptingAndNodes(*j_it)) // For some reason this is being set as a garbage value outside the size of raodsVector BUG
            {
                // If we remove at j then we erase and pass the next valid object back to the iterator
                removedRoadsIntersect++;
                j_it = roadsVector.erase(j_it);
            }
            else {
                // If we dont erase we increment
                j_it++;
            }
        }
    }
    removedRoadsIntersect -= roadsVector.size(); 
    LOG(STATUS, "[" << removedRoadsIntersect << "] roads removed due to intersections.");


    // Create highways before new mini roads
    // When generating, we could give a radius from 0,0 for roads to be permitted, this would give a good effect IMO
    uint64_t timeElapsed = StopWatch::GetTimeElapsed(roadGenerateStartTime);
    LOG(STATUS, "[ GenerateRoads finished. Time elapsed: " << timeElapsed << "ms ]\n");


    // Copy return 
    return roadsVector;
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

std::array<std::string, 9> buildingModelPaths = {
    "../assets/models/Buildings/LowPoly/low_buildingA.obj",
    "../assets/models/Buildings/LowPoly/low_buildingB.obj",
    "../assets/models/Buildings/LowPoly/low_buildingC.obj",
    "../assets/models/Buildings/LowPoly/low_buildingD.obj",
    "../assets/models/Buildings/LowPoly/low_buildingE.obj",
    "../assets/models/Buildings/LowPoly/low_buildingF.obj",
    "../assets/models/Buildings/LowPoly/low_buildingG.obj",
    "../assets/models/Buildings/LowPoly/low_buildingH.obj",
    "../assets/models/Buildings/LowPoly/low_buildingI.obj"
};

void generator::GenerateBuildings(float densityFactor)
{
    LOG(STATUS, "[ Started GeneratedBuildings ]");
    auto buildingGenerateStartTime = StopWatch::GetCurrentTimePoint();

    // TEMP
    // std::string buildingTest = "../assets/models/Buildings/buildingTest2.obj";
    std::string buildingTest = "../assets/models/Buildings/LowPoly/low_buildingC.obj";
    ShaderPath buildingShader = {paths::building_defaultInstancedVertShaderPath, paths::building_defaultFragShaderPath}; 

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
                // If we fall in range of the density factor we add the area for buildings to be placed
                if (Random::GetPercentage() <= densityFactor)
                {
                    areas.push_back(*zone);
                    buildingCount++;
                }
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
                if (Random::GetPercentage() < densityFactor)
                {
                    areas.push_back(*zone);
                    buildingCount++;
                }
            }
            else 
                break;
        }       
    }
    LOG(STATUS, "[" << buildingCount << "] Buildings generated."); 
 
    // Pass to remove overlapping buildings
    int intersectingBuildings = 0;
    for (size_t i = 0; i < areas.size(); i++)
    {
        bool intersects = false;
        for (size_t j = i+1; j < areas.size(); j++)
        {
            if (areas[i].TooFarForCollision(&areas[j]))
            {
                // Move onto next one
                continue;
            }
            if (areas[i].Intersects(areas[j].zoneVerticesArray))
            {
                intersects = true;
                break;
            }
        }
        if (!intersects)
        {
            // Street lights for roads
            if (i % 10 == 0)
            {
                // scene->addPointLight()
                //     ->SetIsVisible(false) // Disable the sprite
                //     ->SetPosition(areas[i].zoneVerticesArray[0])
                //     ;
            }

            // Add random buildings
            scene->addModel(buildingModelPaths[i%9], &buildingShader)
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

