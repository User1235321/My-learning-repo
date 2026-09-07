#include "grid.hpp"

#include <stdexcept>

namespace lab1
{

Grid::Grid(std::vector<double> nodes) : nodes_(std::move(nodes))
{
  if (nodes_.size() < 2)
    throw std::invalid_argument("Grid needs at least two nodes");
  for (std::size_t i = 1; i < nodes_.size(); ++i)
    if (nodes_[i] <= nodes_[i - 1])
      throw std::invalid_argument("Grid nodes must increase");
}

Grid Grid::uniform(double left, double right, int intervals)
{
  if (intervals < 1 || right <= left)
    throw std::invalid_argument("Invalid uniform grid");
  std::vector<double> nodes(static_cast<std::size_t>(intervals) + 1);
  for (int i = 0; i <= intervals; ++i)
    nodes[static_cast<std::size_t>(i)] = left + (right - left) * i / intervals;
  return Grid(std::move(nodes));
}

Grid Grid::from_config(const GridConfig & config)
{
  if (config.type == "uniform")
    return uniform(config.left, config.right, config.intervals);
  if (config.type == "nonuniform")
    return Grid(config.nodes);
  throw std::invalid_argument("Unknown grid type: " + config.type);
}

const std::vector<double> & Grid::nodes() const
{
  return nodes_;
}
std::size_t Grid::size() const
{
  return nodes_.size();
}
double Grid::operator[](std::size_t index) const
{
  return nodes_[index];
}

}
