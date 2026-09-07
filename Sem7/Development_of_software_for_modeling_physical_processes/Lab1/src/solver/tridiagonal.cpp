#include "tridiagonal.hpp"

#include <cmath>
#include <stdexcept>

namespace lab1
{

std::vector<double> solve_tridiagonal(const std::vector<double> & lower, const std::vector<double> & diagonal,
                                      const std::vector<double> & upper, const std::vector<double> & rhs)
{
  const std::size_t n = diagonal.size();
  if (n == 0 || lower.size() != n || upper.size() != n || rhs.size() != n)
    throw std::invalid_argument("Invalid tridiagonal system");
  if (std::abs(diagonal[0]) < 1e-14)
    throw std::runtime_error("Singular tridiagonal system");
  std::vector<double> c(n), d(n), result(n);
  c[0] = upper[0] / diagonal[0];
  d[0] = rhs[0] / diagonal[0];
  for (std::size_t i = 1; i < n; ++i)
  {
    const double denominator = diagonal[i] - lower[i] * c[i - 1];
    if (std::abs(denominator) < 1e-14)
      throw std::runtime_error("Singular tridiagonal system");
    c[i] = i + 1 < n ? upper[i] / denominator : 0.0;
    d[i] = (rhs[i] - lower[i] * d[i - 1]) / denominator;
  }
  result[n - 1] = d[n - 1];
  for (std::size_t i = n - 1; i-- > 0;)
    result[i] = d[i] - c[i] * result[i + 1];
  return result;
}

}
