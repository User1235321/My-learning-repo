#include <iostream>
#include <chrono>
#include <vector>

#include "source.hpp"
#include "sourceBufferDispatcher.hpp"
#include "buffer.hpp"
#include "bufferHandlerDispatcher.hpp"
#include "handler.hpp"

int main()
{
  srand(time(0));
  const double delay = 10.0, handWork = 0.5;
  const double lambda = 0.5;
  const size_t buffSize = 10;

  std::vector< source > sources;
  sources.emplace_back(1, "Мастер спорта", lambda, &std::cout, delay);
  sources.emplace_back(2, "Кандидат в мастера спорта", 1.0, &std::cout, delay);
  sources.emplace_back(3, "Остальные", 5.0, &std::cout, delay);
  buffer buff(&std::cout, delay, delay);
  sourceBufferDispatcher firstDisp(sources, buff, &std::cout, delay);
  std::vector< handler > handlers;
  handlers.emplace_back(&std::cout, delay, handWork);
  handlers.emplace_back(&std::cout, delay, handWork);
  handlers.emplace_back(&std::cout, delay, handWork);
  bufferHandlerDispatcher secondDisp(handlers, buff, &std::cout, buffSize);

  std::cout << "Автоматический режим (1), пошаговый (2)\n";
  int choice = 0;
  std::cin >> choice;

  if (choice == 2)
  {
    double stepTime = 0.25;
    double totalTime = 0.0;
    while (std::cin.get() != 27)
    {
      std::cout << "Время: " << totalTime << '\n';
      for (auto & src : sources)
      {
        src.stepWork(stepTime);
      }
      firstDisp.stepWork();
      secondDisp.stepWork();
      for (auto & hand : handlers)
      {
        hand.stepWork(stepTime);
      }
      totalTime += stepTime;
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
    for (auto & hand : handlers)
    {
      hand.autoWork();
    }
    secondDisp.autoWork();
    std::cin.ignore();
    std::cin.get();
  }

  std::cout << "Итоговая статистика:\n";
  int sum = 0;
  for(size_t i = 0; i < sources.size(); ++i)
  {
    sum += sources[i].getAppNum();
  }
  std::cout << "Всего была сгенерировано " << sum << " заявок. ";
  std::cout << "Из них: " << sources[0].getAppNum() << " - Мастера спорта, ";
  std::cout << sources[1].getAppNum() << " - Кандидаты в мастера спорта, ";
  std::cout << sources[2].getAppNum() << " - остальные.\n";
  std::cout << "Из буфера удалено " << buff.getDelNum() << " заявок\n";
  std::cout << "Менеджерами обработано " << (handlers[0].getAppNum() + handlers[1].getAppNum() + handlers[2].getAppNum()) << " заявок.\n";
  return 0;
}
