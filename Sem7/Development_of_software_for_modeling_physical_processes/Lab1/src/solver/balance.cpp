#include "balance.hpp"
#include "tridiagonal.hpp"
#include <algorithm>
#include <cmath>
namespace lab1
{
std::vector<double> solve_balance(const Grid & grid, const Model & model)
{
  const std::size_t n = grid.size();
  std::vector<double> lower(n), diagonal(n), upper(n), rhs(n);
  for (std::size_t i = 0; i < n; ++i)
  {
    const double coordinate = grid[i];
    const double left = i == 0 ? coordinate : (grid[i] + grid[i - 1]) / 2.0;
    const double right = i + 1 == n ? coordinate : (grid[i] + grid[i + 1]) / 2.0;
    const double control_width = right - left;
    const double weight = control_width * coordinate;
    diagonal[i] = model.q(coordinate) * weight;
    rhs[i] = model.f(coordinate) * weight;
    if (i > 0)
    {
      const double face = (grid[i] + grid[i - 1]) / 2.0;
      const double conductance = face * model.k(face) / (grid[i] - grid[i - 1]);
      lower[i] = -conductance;
      diagonal[i] += conductance;
    }
    else
      rhs[i] += coordinate * model.nu1;
    if (i + 1 < n)
    {
      const double face = (grid[i] + grid[i + 1]) / 2.0;
      const double conductance = face * model.k(face) / (grid[i + 1] - grid[i]);
      upper[i] = -conductance;
      diagonal[i] += conductance;
    }
    else
    {
      diagonal[i] += coordinate * model.chi2;
      rhs[i] += coordinate * model.nu2;
    }
  }
  return solve_tridiagonal(lower, diagonal, upper, rhs);
}
double max_error(const Grid & grid, const Model & model, const std::vector<double> & solution)
{
  double error = 0.0;
  for (std::size_t i = 0; i < grid.size(); ++i)
    error = std::max(error, std::abs(model.exact(grid[i]) - solution[i]));
  return error;
}
}
