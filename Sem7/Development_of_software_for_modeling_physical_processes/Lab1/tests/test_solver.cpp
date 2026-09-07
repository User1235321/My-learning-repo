#include "balance.hpp"
#include "configuration.hpp"
#include "csv_writer.hpp"
#include "experiments.hpp"
#include "grid.hpp"
#include "model.hpp"
#include "tridiagonal.hpp"

#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
int failures = 0;

void check(const std::string & name, const std::function<void()> & test)
{
  std::cout << name << std::endl;
  try
  {
    test();
    std::cout << "  PASS" << std::endl;
  }
  catch (const std::exception & error)
  {
    ++failures;
    std::cout << "  FAIL: " << error.what() << std::endl;
  }
}

void require(bool condition, const std::string & message)
{
  if (!condition)
    throw std::runtime_error(message);
}

void require_close(double actual, double expected, double tolerance, const std::string & message)
{
  require(std::abs(actual - expected) <= tolerance, message);
}

void require_residual(const std::vector<double> & lower, const std::vector<double> & diagonal,
                      const std::vector<double> & upper, const std::vector<double> & rhs,
                      const std::vector<double> & solution)
{
  for (std::size_t i = 0; i < solution.size(); ++i)
  {
    const double value = diagonal[i] * solution[i] + (i == 0 ? 0.0 : lower[i] * solution[i - 1]) +
                         (i + 1 == solution.size() ? 0.0 : upper[i] * solution[i + 1]);
    require_close(value, rhs[i], 1e-10, "tridiagonal residual");
  }
}

}

int main()
{
  check("tridiagonal systems",
        []
        {
          const std::vector<double> empty;
          const auto one = lab1::solve_tridiagonal({0.0}, {4.0}, {0.0}, {12.0});
          require_close(one[0], 3.0, 1e-12, "one by one system");
          const std::vector<double> lower{0.0, -1.0, -1.0, -1.0};
          const std::vector<double> diagonal{4.0, 4.0, 4.0, 4.0};
          const std::vector<double> upper{-1.0, -1.0, -1.0, 0.0};
          const std::vector<double> rhs{6.0, 4.0, 4.0, 6.0};
          const auto solution = lab1::solve_tridiagonal(lower, diagonal, upper, rhs);
          for (double value : solution)
            require_close(value, 2.0, 1e-12, "dominant tridiagonal solution");
          require_residual(lower, diagonal, upper, rhs, solution);
        });

  check("invalid tridiagonal inputs",
        []
        {
          const std::vector<double> empty;
          bool thrown = false;
          try
          {
            lab1::solve_tridiagonal(empty, empty, empty, empty);
          }
          catch (const std::invalid_argument &)
          {
            thrown = true;
          }
          require(thrown, "empty system must be rejected");
          thrown = false;
          try
          {
            lab1::solve_tridiagonal({0.0}, {0.0}, {0.0}, {1.0});
          }
          catch (const std::exception &)
          {
            thrown = true;
          }
          require(thrown, "zero leading element must be rejected");
        });

  check("uniform and nonuniform grids",
        []
        {
          const auto uniform = lab1::Grid::uniform(1.0, 2.0, 4);
          require(uniform.size() == 5, "uniform size");
          require_close(uniform[1] - uniform[0], 0.25, 1e-12, "uniform step");
          const auto nonuniform = lab1::Grid({1.0, 1.1, 1.4, 2.0});
          require(nonuniform.size() == 4, "nonuniform size");
          require_close(nonuniform[2] - nonuniform[1], 0.3, 1e-12, "nonuniform step");
          bool thrown = false;
          try
          {
            lab1::Grid({1.0, 1.0});
          }
          catch (const std::invalid_argument &)
          {
            thrown = true;
          }
          require(thrown, "nonmonotonic grid must be rejected");
          thrown = false;
          try
          {
            lab1::Grid::uniform(2.0, 1.0, 4);
          }
          catch (const std::invalid_argument &)
          {
            thrown = true;
          }
          require(thrown, "reversed uniform grid must be rejected");
        });

  check("zero-error manufactured solution",
        []
        {
          const auto model = lab1::make_model("zero_error_test", 1.0, 2.0);
          for (int intervals : {2, 7, 20})
          {
            const auto grid = lab1::Grid::uniform(1.0, 2.0, intervals);
            const auto solution = lab1::solve_balance(grid, model);
            require(lab1::max_error(grid, model, solution) < 1e-12, "constant manufactured solution");
          }
        });

  check("nonzero-error convergence and boundaries",
        []
        {
          const auto model = lab1::make_model("nonzero_error_test", 1.0, 2.0);
          double previous = 0.0;
          for (int intervals : {20, 40, 80})
          {
            const auto grid = lab1::Grid::uniform(1.0, 2.0, intervals);
            const auto solution = lab1::solve_balance(grid, model);
            const double error = lab1::max_error(grid, model, solution);
            require(error > 0.0, "nonzero manufactured error");
            if (previous > 0.0)
              require(error < previous / 3.0, "approximately second-order convergence");
            require(std::abs(solution.front() - model.exact(1.0)) < 0.01, "left boundary");
            require(std::abs(solution.back() - model.exact(2.0)) < 0.01, "right boundary");
            previous = error;
          }
        });

  check("cylindrical weight and Robin sign",
        []
        {
          lab1::Model model;
          model.k = [](double r) { return 1.0 / r; };
          model.q = [](double) { return 0.0; };
          model.f = [](double) { return 0.0; };
          model.exact = [](double r) { return r; };
          model.nu1 = -1.0;
          model.chi2 = 2.0;
          model.nu2 = 4.5;
          const auto grid = lab1::Grid::uniform(1.0, 2.0, 9);
          const auto solution = lab1::solve_balance(grid, model);
          require(lab1::max_error(grid, model, solution) < 1e-11, "weighted flux manufactured solution");
        });

  check("balance control-volume quadrature",
        []
        {
          const lab1::Grid grid({1.0, 1.3, 1.9, 2.8});
          lab1::Model model;
          model.k = [](double) { return 1.0; };
          model.q = [](double) { return 1.0; };
          model.f = [](double) { return 2.0; };
          model.exact = [](double) { return 0.0; };
          const auto actual = lab1::solve_balance(grid, model);
          const std::size_t n = grid.size();
          std::vector<double> lower(n), diagonal(n), upper(n), rhs(n);
          for (std::size_t i = 0; i < n; ++i)
          {
            const double left_step = i == 0 ? grid[1] - grid[0] : grid[i] - grid[i - 1];
            const double right_step = i + 1 == n ? grid[n - 1] - grid[n - 2] : grid[i + 1] - grid[i];
            const double volume = grid[i] * (left_step + right_step) / 2.0;
            diagonal[i] = volume;
            rhs[i] = 2.0 * volume;
            if (i > 0)
            {
              const double face = (grid[i] + grid[i - 1]) / 2.0;
              const double conductance = face / (grid[i] - grid[i - 1]);
              lower[i] = -conductance;
              diagonal[i] += conductance;
            }
            if (i + 1 < n)
            {
              const double face = (grid[i] + grid[i + 1]) / 2.0;
              const double conductance = face / (grid[i + 1] - grid[i]);
              upper[i] = -conductance;
              diagonal[i] += conductance;
            }
          }
          const auto expected = lab1::solve_tridiagonal(lower, diagonal, upper, rhs);
          for (std::size_t i = 0; i < n; ++i)
            require_close(actual[i], expected[i], 1e-10, "rectangular weighted volume");
        });

  check("configuration parser",
        []
        {
          const auto uniform = lab1::load_configuration("Lab1/input/config.txt");
          require(uniform.grid.type == "uniform", "uniform config type");
          require(lab1::Grid::from_config(uniform.grid).size() > 2, "uniform config grid");
          const auto nonuniform = lab1::load_configuration("Lab1/input/zero_error_config.txt");
          require(nonuniform.grid.type == "nonuniform", "nonuniform config type");
          require(lab1::Grid::from_config(nonuniform.grid).size() == nonuniform.grid.nodes.size(),
                  "nonuniform config grid");
          bool thrown = false;
          try
          {
            lab1::load_configuration("/tmp/lab1_missing_config");
          }
          catch (const std::runtime_error &)
          {
            thrown = true;
          }
          require(thrown, "missing config must be rejected");
          lab1::GridConfig invalid;
          invalid.type = "other";
          thrown = false;
          try
          {
            lab1::Grid::from_config(invalid);
          }
          catch (const std::invalid_argument &)
          {
            thrown = true;
          }
          require(thrown, "unknown grid type must be rejected");
        });

  check("nonuniform experiment bounds",
        []
        {
          const std::string path = "/tmp/lab1_nonuniform_experiment.conf";
          std::ofstream output(path);
          output << "model=zero_error_test\ngrid=nonuniform\nnodes=3,3.4,4.2\nexperiment_intervals=2\n";
          output.close();
          const auto config = lab1::load_configuration(path);
          lab1::run_experiments(config);
          std::ifstream input("Lab1/results/convergence.csv");
          std::string line;
          std::getline(input, line);
          std::getline(input, line);
          require(line.rfind("2,0.6,", 0) == 0, "experiment bounds from nonuniform nodes");
        });

  check("CSV headers and row counts",
        []
        {
          const auto grid = lab1::Grid::uniform(1.0, 2.0, 3);
          const auto model = lab1::make_model("zero_error_test", 1.0, 2.0);
          const auto solution = lab1::solve_balance(grid, model);
          const std::string path = "/tmp/lab1_solution_test.csv";
          lab1::write_solution_csv(path, grid, model, solution);
          std::ifstream input(path);
          std::string line;
          std::getline(input, line);
          require(line == "coordinate,numerical,exact,absolute_error", "solution CSV header");
          int rows = 0;
          while (std::getline(input, line))
            ++rows;
          require(rows == static_cast<int>(grid.size()), "solution CSV rows");
          lab1::write_convergence_csv("/tmp/lab1_convergence_test.csv", {2, 4}, {0.5, 0.25}, {1.0, 0.25});
          input.close();
          input.open("/tmp/lab1_convergence_test.csv");
          std::getline(input, line);
          require(line == "intervals,step,error", "convergence CSV header");
          rows = 0;
          while (std::getline(input, line))
            ++rows;
          require(rows == 2, "convergence CSV rows");
          lab1::write_performance_csv("/tmp/lab1_performance_test.csv", {2, 4}, {1.0, 2.0});
          input.close();
          input.open("/tmp/lab1_performance_test.csv");
          std::getline(input, line);
          require(line == "intervals,time_us", "performance CSV header");
          rows = 0;
          while (std::getline(input, line))
            ++rows;
          require(rows == 2, "performance CSV rows");
        });

  std::cout << "Failures: " << failures << std::endl;
  return failures == 0 ? 0 : 1;
}
