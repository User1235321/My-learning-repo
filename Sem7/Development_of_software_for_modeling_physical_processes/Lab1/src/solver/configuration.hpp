#pragma once

#include <string>
#include <vector>

namespace lab1
{

struct GridConfig
{
    std::string type = "uniform";
    double left = 1.0;
    double right = 2.0;
    int intervals = 10;
    std::vector<double> nodes;
};

struct Configuration
{
    std::string model = "zero_error_test";
    GridConfig grid;
    std::vector<int> experiment_intervals;
    int performance_repeats = 20;
};

Configuration load_configuration(const std::string & path);

}
