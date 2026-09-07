#include "solver/configuration.hpp"
#include "solver/experiments.hpp"
#include <exception>
#include <iostream>
int main(int argc, char ** argv)
{
  if (argc != 2)
  {
    std::cerr << "Usage: solver <configuration>\n";
    return 1;
  }
  try
  {
    lab1::run_experiments(lab1::load_configuration(argv[1]));
  }
  catch (const std::exception & error)
  {
    std::cerr << error.what() << '\n';
    return 1;
  }
}
