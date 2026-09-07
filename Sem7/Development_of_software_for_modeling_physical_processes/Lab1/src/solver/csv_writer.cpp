#include "csv_writer.hpp"
#include <cmath>
#include <fstream>
namespace lab1
{
void write_solution_csv(const std::string & path, const Grid & grid, const Model & model,
                        const std::vector<double> & solution)
{
  std::ofstream output(path);
  output << "coordinate,numerical,exact,absolute_error\n";
  for (std::size_t i = 0; i < grid.size(); ++i)
  {
    const double exact = model.exact(grid[i]);
    output << grid[i] << ',' << solution[i] << ',' << exact << ',' << std::abs(solution[i] - exact) << '\n';
  }
}
void write_convergence_csv(const std::string & path, const std::vector<int> & intervals,
                           const std::vector<double> & steps, const std::vector<double> & errors)
{
  std::ofstream output(path);
  output << "intervals,step,error\n";
  for (std::size_t i = 0; i < intervals.size(); ++i)
    output << intervals[i] << ',' << steps[i] << ',' << errors[i] << '\n';
}
void write_performance_csv(const std::string & path, const std::vector<int> & intervals,
                           const std::vector<double> & times)
{
  std::ofstream output(path);
  output << "intervals,time_us\n";
  for (std::size_t i = 0; i < intervals.size(); ++i)
    output << intervals[i] << ',' << times[i] << '\n';
}
}
