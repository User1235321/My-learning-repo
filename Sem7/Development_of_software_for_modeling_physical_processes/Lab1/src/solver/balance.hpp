#pragma once
#include "grid.hpp"
#include "model.hpp"
#include <vector>
namespace lab1
{
std::vector<double> solve_balance(const Grid &, const Model &);
double max_error(const Grid &, const Model &, const std::vector<double> &);
}
