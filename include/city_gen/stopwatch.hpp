#pragma once
/*
    Class is to measure the runtime of the program and give info to the logging system
*/

#include <chrono>

using high_res_clock = std::chrono::high_resolution_clock;

class StopWatch{
public:
    static high_res_clock::time_point start;

    static int64_t GetTimeElapsed()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(high_res_clock::now() - start).count();
    }

    // For custom timing such as generation elapsed time.
    static int64_t GetTimeElapsed(high_res_clock::time_point start_time)
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(high_res_clock::now() - start_time).count();
    }

    // Get time point
    static high_res_clock::time_point GetCurrentTimePoint(void) 
    {
        return std::chrono::high_resolution_clock::now();
    }

};
