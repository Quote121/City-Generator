#include <cityRandom.hpp>
#include <cstdlib>

int Random::GetIntBetweenInclusive(int a, int b)
{
    return a + (std::rand() % (b-a));
}

float Random::GetFloatBetweenInclusive(float a, float b)
{
    float low, high;
    if (a < b) { low = a; high = b; }
    else { low = b; high = a; }

    return low + static_cast<float>(rand()) /(static_cast<float>(RAND_MAX/(high-low)));
}

float Random::GetPercentage(void)
{
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

