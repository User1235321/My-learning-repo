#include <iostream>
#include <chrono>
#include <vector>

#include "mainSystem/buffer.hpp"
#include "mainSystem/bufferHandlerDispatcher.hpp"
#include "mainSystem/handler.hpp"
#include "additionalSystem/inits.hpp"
#include "additionalSystem/printer.hpp"
#include "mainSystem/source.hpp"
#include "mainSystem/sourceBufferDispatcher.hpp"

int main()
{
  srand(time(0));
  const size_t buffSize = 10, numOfSrc = 3, numOfBuffs = 1, numOfHands = 5, numOfFDisps = 1, numOfSDisps = 1;
  const long double delay = 0, handWorkCONST = 0.07, lambdaCONST = 5, stepTime = 0.025;
  std::vector< long double > lambda{lambdaCONST, lambdaCONST, lambdaCONST, lambdaCONST * 5.5, lambdaCONST * 6.0};
  std::vector< std::string > sourcesNames{"МС", "КМС", "I разряд", "II разряд", "III разряд"};
  for (size_t i = sourcesNames.size(); i < numOfSrc; ++i)
  {
    sourcesNames.emplace_back(std::string("Дополнительная категория №") + std::to_string(i - sourcesNames.size() + 1));
    lambda.emplace_back(lambdaCONST * 10.0);
  }
  std::vector< long double > handWork;//{0.13, 0.13, 0.1, 0.07, 0.07};
  for (size_t i = handWork.size(); i < numOfHands; ++i)
  {
    handWork.emplace_back(handWorkCONST);
  }
  printer print(numOfSrc, sourcesNames, numOfBuffs, numOfHands, buffSize, &std::cout);

  std::vector< source > sources(initSources(numOfSrc, lambda, delay, &print));
  std::vector< buffer > buff(initBuffers(numOfBuffs, buffSize, &print));
  std::vector< handler > handlers(initHandlers(numOfHands, delay, handWork, &print));
  std::vector< sourceBufferDispatcher > firstDisp(initFDisps(numOfFDisps, &sources, &buff, delay));
  std::vector< bufferHandlerDispatcher > secondDisp(initSDisps(numOfSDisps, &handlers, &buff, delay));

  std::cout << "Пошаговый режим (1), автоматический (2)\n";
  int choice = 0;
  std::cin >> choice;

  print.setMod(choice - 1);
  if (choice == 1)
  {
    size_t step = 0;
    long double totalTime = 0.0;
    while (std::cin.get() != 27)
    {
      for (auto & src : sources)
        src.stepWork(stepTime);
      for (auto & fDisp : firstDisp)
        fDisp.stepWork();
      for (auto & sDisp : secondDisp)
        sDisp.stepWork();
      for (auto & hand : handlers)
        hand.stepWork(stepTime);
      print.printStep(++step, totalTime);
      totalTime += stepTime;
    }
  }
  else
  {
    auto start = std::chrono::high_resolution_clock::now();
    for (auto & src : sources)
      src.autoWork();
    for (auto & fDisp : firstDisp)
      fDisp.autoWork();
    for (auto & hand : handlers)
      hand.autoWork();
    for (auto & sDisp : secondDisp)
      sDisp.autoWork();

    while ((std::chrono::duration< long double >(std::chrono::high_resolution_clock::now() - start).count() < 10)){}
    //std::cin.ignore();
    //std::cin.get();
    for (auto & src : sources)
      src.stopAutoWork();
    for (auto & fDisp : firstDisp)
        fDisp.stopAutoWork();
    for (auto & sDisp : secondDisp)
        sDisp.stopAutoWork();
    for (auto & hand : handlers)
        hand.stopAutoWork();
    print.printRes(std::chrono::duration< long double >(std::chrono::high_resolution_clock::now() - start).count());
  }

  return 0;
}
