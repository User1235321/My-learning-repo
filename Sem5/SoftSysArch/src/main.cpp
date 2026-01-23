#include <iostream>
#include <chrono>
#include <vector>
#include <fstream>
#include <mutex>

#include "source.hpp"
#include "sourceBufferDispatcher.hpp"
#include "buffer.hpp"
#include "bufferHandlerDispatcher.hpp"
#include "handler.hpp"
#include "printer.hpp"

int main()
{
  srand(time(0));
  const double delay = 100.0, handWork = 1;
  const double lambda = 0.5;
  const size_t buffSize = 10;
  const size_t numOfBuffs = 1, numOfHands = 3, numOfSrc = 3;
  std::ofstream outFile("log.txt");
  std::mutex outMutex;

  std::vector< source > sources;
  sources.emplace_back(1, "Мастер спорта", lambda, &outFile, delay, &outMutex);
  sources.emplace_back(2, "Кандидат в мастера спорта", (lambda / 2.0), &outFile, delay, &outMutex);
  sources.emplace_back(3, "Остальные", (lambda / 5.0), &outFile, delay, &outMutex);
  for (size_t i = 0; i < (numOfSrc - 3); ++i)
  {
    std::string name = std::to_string(i + 4) + std::string(" источник");
    sources.emplace_back(i + 4, name, lambda, &outFile, delay, &outMutex);
  }
  std::vector< buffer > buff;
  for (size_t i = 0; i < numOfBuffs; ++i)
  {
    buff.emplace_back(&outFile, buffSize, delay, numOfSrc, &outMutex);
  }
  std::vector< sourceBufferDispatcher > firstDisp;
  firstDisp.emplace_back(sources, buff, &outFile, delay, &outMutex);
  std::vector< handler > handlers;
  for (size_t i = 0; i < numOfHands; ++i)
  {
    handlers.emplace_back(&outFile, delay, handWork, numOfSrc, &outMutex);
  }
  std::vector< bufferHandlerDispatcher > secondDisp;
  secondDisp.emplace_back(handlers, buff, &outFile, buffSize, &outMutex);

  std::cout << "Автоматический режим (1), пошаговый (2)\n";
  int choice = 0;
  std::cin >> choice;

  if (choice == 2)
  {
    double stepTime = 0.25, totalTime = 0.0;
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
      printForStep(std::cout, buff, handlers, ++step);
      totalTime += stepTime;
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
    std::cin.ignore();
    std::cin.get();
    for (auto & src : sources)
    {
      src.stopAutoWork();
    }
    for (auto & fDisp : firstDisp)
    {
      fDisp.stopAutoWork();
    }
    for (auto & hand : handlers)
    {
      hand.stopAutoWork();
    }
    for (auto & sDisp : secondDisp)
    {
      sDisp.stopAutoWork();
    }
    printRes(std::cout, buff, handlers, sources);
  }
  return 0;
}
