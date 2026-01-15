#include <iostream>
#include <chrono>
#include <vector>

#include "source.hpp"
#include "sourceBufferDispatcher.hpp"
#include "buffer.hpp"

int main()
{
  srand(time(0));

  std::vector< source > sources;
  sources.emplace_back(1, "Мастер спорта", 0.5, &std::cout);
  sources.emplace_back(2, "Кандидат в мастера спорта", 1.0, &std::cout);
  sources.emplace_back(3, "Остальные", 2.0, &std::cout);
  buffer buff;
  sourceBufferDispatcher firstDisp(sources, buff, &std::cout);

  std::cout << "Автоматический режим (1), пошаговый (2)\n";
  int choice = 0;
  std::cin >> choice;

  if (choice == 2)
  {
    double stepTime = 0.25;
    for (double totalTime = 0.0; totalTime < 8; totalTime += stepTime)
    {
      std::cout << "Время: " << totalTime << '\n';
      for (auto & src : sources)
      {
        src.stepWork(stepTime);
      }
      firstDisp.stepWork();
    }
  }
  else
  {
    std::cout << "\nНажмите Enter для остановки...\n";
    for (auto & src : sources)
    {
      src.autoWork();
    }
    firstDisp.autoWork();
    std::cin.ignore();
    std::cin.get();
    for (auto & src : sources)
    {
      src.stopAutoWork();
    }
  }

  return 0;
}
