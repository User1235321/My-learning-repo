#pragma once

#include <vector>

namespace lab1
{

std::vector<double> solve_tridiagonal(const std::vector<double> & lower, const std::vector<double> & diagonal,
                                      const std::vector<double> & upper, const std::vector<double> & rhs);

}
