#include <iostream>
#include <chrono>
#include <vector>
#include <fstream>

#include "source.hpp"
#include "sourceBufferDispatcher.hpp"
#include "buffer.hpp"
#include "bufferHandlerDispatcher.hpp"
#include "handler.hpp"
#include "print.hpp"

int main()
{
  srand(time(0));
  const double delay = 10.0, handWork = 0.5;
  const double lambda = 0.5;
  const size_t buffSize = 10;
  std::ofstream outFile("log.txt");

  std::vector< source > sources;
  sources.emplace_back(1, "Мастер спорта", lambda, &outFile, delay);
  sources.emplace_back(2, "Кандидат в мастера спорта", 1.0, &outFile, delay);
  sources.emplace_back(3, "Остальные", 5.0, &outFile, delay);
  std::vector< buffer > buff;
  buff.emplace_back(&outFile, delay, delay);
  std::vector< sourceBufferDispatcher > firstDisp;
  firstDisp.emplace_back(sources, buff, &outFile, delay);
  std::vector< handler > handlers;
  handlers.emplace_back(&outFile, delay, handWork);
  handlers.emplace_back(&outFile, delay, handWork);
  handlers.emplace_back(&outFile, delay, handWork);
  std::vector< bufferHandlerDispatcher > secondDisp;
  secondDisp.emplace_back(handlers, buff, &outFile, buffSize);

  std::cout << "Автоматический режим (1), пошаговый (2)\n";
  int choice = 0;
  std::cin >> choice;

  if (choice == 2)
  {
    double stepTime = 0.25;
    double totalTime = 0.0;
    size_t step = 0;
    while (std::cin.get() != 27)
    {
      std::cout << "Время: " << totalTime << '\n';
      for (auto & src : sources)
      {
        src.stepWork(stepTime);
      }
      for (auto & fDisp : firstDisp)
      {
        fDisp.stepWork();
      }
      for (auto & sDisp : secondDisp)
      {
        sDisp.stepWork();
      }
      for (auto & hand : handlers)
      {
        hand.stepWork(stepTime);
      }
      printForStep(std::cout, buff, handlers, step);
      totalTime += stepTime;
      ++step;
    }
  }
  else
  {
    for (auto & fDisp : firstDisp)
    {
      fDisp.replaceOut(&std::cout);
    }
    for (auto & sDisp : secondDisp)
    {
      sDisp.replaceOut(&std::cout);
    }
    for (auto & bff : buff)
    {
      bff.replaceOut(&std::cout);
    }
    for (auto & src : sources)
    {
      src.replaceOut(&std::cout);
    }
    for (auto & hand : handlers)
    {
      hand.replaceOut(&std::cout);
    }
    std::cout << "\nНажмите Enter для остановки...\n";
    for (auto & src : sources)
    {
      src.autoWork();
    }
    for (auto & fDisp : firstDisp)
    {
      fDisp.autoWork();
    }
    for (auto & hand : handlers)
    {
      hand.autoWork();
    }
    for (auto & sDisp : secondDisp)
    {
      sDisp.autoWork();
    }
    std::atomic< bool > printing{true};
    std::cin.ignore();
    std::cin.get();
    printing = false;
  }
  printRes(std::cout, buff, handlers, sources);
  return 0;
}
