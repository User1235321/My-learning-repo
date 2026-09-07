#include "configuration.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>

namespace lab1
{
namespace
{

std::string trim(std::string value)
{
  const auto first = value.find_first_not_of(" \t\r\n");
  if (first == std::string::npos)
    return {};
  const auto last = value.find_last_not_of(" \t\r\n");
  return value.substr(first, last - first + 1);
}

std::vector<double> parse_numbers(const std::string & value)
{
  std::vector<double> result;
  std::stringstream stream(value);
  std::string token;
  while (std::getline(stream, token, ','))
    result.push_back(std::stod(trim(token)));
  return result;
}

}

Configuration load_configuration(const std::string & path)
{
  std::ifstream input(path);
  if (!input)
    throw std::runtime_error("Cannot open configuration: " + path);
  Configuration config;
  std::string line;
  while (std::getline(input, line))
  {
    const auto comment = line.find('#');
    if (comment != std::string::npos)
      line.resize(comment);
    const auto separator = line.find('=');
    if (separator == std::string::npos)
      continue;
    const auto key = trim(line.substr(0, separator));
    const auto value = trim(line.substr(separator + 1));
    if (key == "model")
      config.model = value;
    else if (key == "grid")
      config.grid.type = value;
    else if (key == "left")
      config.grid.left = std::stod(value);
    else if (key == "right")
      config.grid.right = std::stod(value);
    else if (key == "intervals")
      config.grid.intervals = std::stoi(value);
    else if (key == "nodes")
      config.grid.nodes = parse_numbers(value);
    else if (key == "experiment_intervals")
    {
      for (const auto number : parse_numbers(value))
        config.experiment_intervals.push_back(static_cast<int>(number));
    }
    else if (key == "performance_repeats")
      config.performance_repeats = std::stoi(value);
  }
  return config;
}

}
