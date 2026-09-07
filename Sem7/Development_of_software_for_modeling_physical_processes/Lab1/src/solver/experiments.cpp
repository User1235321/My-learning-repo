#include "experiments.hpp"
#include "balance.hpp"
#include "csv_writer.hpp"
#include "grid.hpp"
#include "model.hpp"
#include <chrono>
#include <filesystem>
namespace lab1
{
void run_experiments(const Configuration & config)
{
  std::filesystem::create_directories("Lab1/results");
  const Grid base_grid = Grid::from_config(config.grid);
  const double left = base_grid[0];
  const double right = base_grid[base_grid.size() - 1];
  const Model base_model = make_model(config.model, left, right);
  write_solution_csv("Lab1/results/solution.csv", base_grid, base_model, solve_balance(base_grid, base_model));
  std::vector<int> intervals = config.experiment_intervals;
  if (intervals.empty())
    intervals.push_back(static_cast<int>(base_grid.size() - 1));
  std::vector<double> steps, errors, times;
  for (const int count : intervals)
  {
    const Grid grid = Grid::uniform(left, right, count);
    const Model model = make_model(config.model, left, right);
    const auto values = solve_balance(grid, model);
    steps.push_back((right - left) / count);
    errors.push_back(max_error(grid, model, values));
    const auto start = std::chrono::steady_clock::now();
    for (int repeat = 0; repeat < config.performance_repeats; ++repeat)
      solve_balance(grid, model);
    const auto finish = std::chrono::steady_clock::now();
    times.push_back(std::chrono::duration<double, std::micro>(finish - start).count() / config.performance_repeats);
  }
  write_convergence_csv("Lab1/results/convergence.csv", intervals, steps, errors);
  write_performance_csv("Lab1/results/performance.csv", intervals, times);
}
}
