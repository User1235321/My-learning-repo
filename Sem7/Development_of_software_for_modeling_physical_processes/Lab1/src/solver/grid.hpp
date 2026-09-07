#pragma once

#include "configuration.hpp"

#include <vector>

namespace lab1
{

class Grid
{
  public:
    explicit Grid(std::vector<double> nodes);
    static Grid uniform(double left, double right, int intervals);
    static Grid from_config(const GridConfig & config);
    const std::vector<double> & nodes() const;
    std::size_t size() const;
    double operator[](std::size_t index) const;

  private:
    std::vector<double> nodes_;
};

}
