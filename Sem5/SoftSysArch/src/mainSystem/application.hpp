#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <chrono>

struct application
{
    size_t id_, priority_, sourceId_;
    std::chrono::time_point< std::chrono::high_resolution_clock > startTime_;
};

#endif
