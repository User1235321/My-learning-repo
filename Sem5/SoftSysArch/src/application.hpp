#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <chrono>

struct application
{
    int priority_, id_, actualStageNum_;
    std::chrono::time_point< std::chrono::high_resolution_clock > startTime_;
};

#endif
