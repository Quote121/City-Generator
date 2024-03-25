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

    // @brief Returns an integer between a and b
    // @args a - bound1
    // @args b - bound2
    // @returns int between upper and lower inclusive
    static int GetIntBetweenInclusive(int a, int b);
    
    // @brief Returns a float between a and b inclusive
    // @args a - bound1
    // @args b - bound2
    // @returns float between upper and lower inclusive
    static float GetFloatBetweenInclusive(float a, float b);

    // @brief Return a float between 0 and 1 inclusive
    static float GetPercentage(void);
};

