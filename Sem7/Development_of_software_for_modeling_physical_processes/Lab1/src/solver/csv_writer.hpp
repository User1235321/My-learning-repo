#pragma once
#include "grid.hpp"
#include "model.hpp"
#include <string>
#include <vector>
namespace lab1
{
void write_solution_csv(const std::string &, const Grid &, const Model &, const std::vector<double> &);
void write_convergence_csv(const std::string &, const std::vector<int> &, const std::vector<double> &,
                           const std::vector<double> &);
void write_performance_csv(const std::string &, const std::vector<int> &, const std::vector<double> &);
}
