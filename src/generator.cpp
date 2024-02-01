#include <generator.hpp>
#include <map>
#include <sstream>    
#include <stack>
#include <set>

#include <stopwatch.hpp>

void generator::generateRoads(void)
{
    auto roadGenerateStartTime = StopWatch::GetCurrentTimePoint();


    LOG(STATUS, "generateRoads() started...");
    std::vector<glm::vec3> points;

    // TODO, when working we should have a menu handle to tweak all these values and then to generate. That would be cool

    // Procedural generation of roads here
    //
    // Scene::getInstance()->addRoad(glm::vec3{0, 0, 0}, glm::vec3{5, 0, 5});
    //

    // Seed should be changed based on system time, seed can be saved to replicate the generation
    const siv::PerlinNoise::seed_type seed = 123456u;
    const siv::PerlinNoise perlin{ seed };

    // Note for terrain generation the terrain asset is 160 by 160
    for (int y = 0; y < 160; ++y)
    {
        for (int x = 0; x < 160; ++x)
        {
            const double noise = perlin.normalizedOctave2D_01((x), (y), 4);

            // Random threshold to create a 
            if (noise > 0.65)
            {
                float xPos, yPos;
                xPos = (2*x)-160;
                yPos = (2*y)-160;
                
                // Get Starting points
                points.push_back(glm::vec3{xPos, 0, yPos});
            }
        }
    }

    // Now we will select several points from the vector of points to begin our L-system and any 
    // nodes close to one another will be joined up

    // grammar
    std::string treeGrammar = "F"; // Needs to be changed based on what our grammar is
    generator::LSystemGen(&treeGrammar, 3); // Get 2 iterations on the grammar
    LOG(STATUS, "Grammar: " << treeGrammar)
    // // Using the same seed as before
    // srand(seed);
    // 
    // // Number of points we pick
    // int startingPoints = 4;
    // for (int i = 0; i < startingPoints; i++)
    // {
    //     // Get random point
    //     glm::vec3 point = points.at(rand() % points.size());
    //     // We perfrom the grammar on it. At each node we push it into a stack       
    //     
    // }
    //
    std::stack<road_gen_point> pointStack;
    // std::vector<road_gen_point> endPoints; // End nodes of the tree
    std::set<road_gen_point> endPoints; // End nodes of the tree


    // Test point
    glm::vec3 startPoint = {0.0, 0.0, 0.0};
    road_gen_point currentPoint = {startPoint, 0.0f}; // Heading of 0 radians
    float length = 10.0f;
    
    float degrees = 45.0f;
    float degreeRadians = degrees * (M_PI/180);
    

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
                currentPoint.point.x + (length * glm::sin(currentPoint.degreeHeading)),
                currentPoint.point.y,
                currentPoint.point.z + (length * glm::cos(currentPoint.degreeHeading))
            };

            // Scene::getInstance()->addRoad({0,0,0}, {0, 0, 10});
            Scene::getInstance()->addRoad(currentPoint.point, nextPoint);
            currentPoint.point = nextPoint; // Update current point, no change to bearing

            break;
        }
        case 'X':
        {
            // Go forward relative to its angle
            glm::vec3 nextPoint = {
                currentPoint.point.x + (length * glm::sin(currentPoint.degreeHeading)),
                currentPoint.point.y,
                currentPoint.point.z + (length * glm::cos(currentPoint.degreeHeading))
            };
            
            Scene::getInstance()->addRoad(currentPoint.point, nextPoint);
            
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

    for (int j = 0; j < iterations; j++)
    {
        std::stringstream ss;
        // Current L system grammar
        std::map<const std::string, const std::string> grammar =

            // {{"X", "F[+X]F[-X]F[-X]F[+X]F"},
             // {"F", "FF"}}; 
            {{"F", "F-F++F-F"}};

        for (int i = 0; i < axiom->size(); i++)
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


