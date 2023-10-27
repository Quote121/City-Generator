#pragma once
/*
    Class is to measure the runtime of the program and give info to the logging system
*/

#include <chrono>
#include <iostream>

using high_res_clock = std::chrono::high_resolution_clock;

class StopWatch{
public:
    static high_res_clock::time_point start;

    static int64_t GetTimeElapsed()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(high_res_clock::now() - start).count();
    }
};
