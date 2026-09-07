#pragma once

#include <functional>
#include <string>

namespace lab1
{

struct Model
{
    std::function<double(double)> k;
    std::function<double(double)> q;
    std::function<double(double)> f;
    std::function<double(double)> exact;
    double nu1 = 0.0;
    double chi2 = 0.0;
    double nu2 = 0.0;
};

Model make_model(const std::string & name, double left, double right);

}
