#pragma once

#include <random>
// #include <limits>

class Random {

public:

    // @brief seed generator, used to seed my rand() for generator.cpp
    // @returns int - seed
    static int GenerateSeed(void)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distribution(0, INT32_MAX);
        return distribution(gen);
    }

    // Returns an integer between a and b
    static int GetIntBetweenInclusive(int a, int b);
    
    // Returns a float between a and b inclusive
    static float GetFloatBetweenInclusive(float a, float b);

    // Return a float between 0 and 1 inclusive
    static float GetPercentage(void);
};

