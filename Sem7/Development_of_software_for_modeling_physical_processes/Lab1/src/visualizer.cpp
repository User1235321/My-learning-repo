#include "visualizer/visualizer.hpp"

#include <iostream>
#include <stdexcept>

int main(int argc, char * argv[])
{
  if (argc < 3 || argc > 4 || (argc == 4 && std::string(argv[3]) != "--show"))
  {
    std::cerr << "Использование: visualizer <input.csv> <output.png> [--show]\n";
    return 2;
  }
  try
  {
    const DataSet data = readData(argv[1]);
    renderPlot(data, argv[2], findFont());
    if (argc == 4)
      showPlot(argv[2]);
  }
  catch (const std::exception & error)
  {
    std::cerr << "Ошибка: " << error.what() << '\n';
    return 1;
  }
  return 0;
}
