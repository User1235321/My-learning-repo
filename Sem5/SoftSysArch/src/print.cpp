#include "print.hpp"

void printForStep(std::ostream & out, const std::vector< buffer > & buff, const std::vector< handler > & handlers, size_t step)
{
  out << "ШАГ " << step << '\n';
  for(size_t i = 0; i < buff.size(); ++i)
  {
    out << "В буфере " << (i + 1) << " занято " << buff[i].getFullness() << " ячеек из " << buff[i].getLimit() << '\n';
  }
  for(size_t i = 0; i < handlers.size(); ++i)
  {
    out << "Менеджер №" << (i + 1);
    if (handlers[i].free())
    {
      out << " не";
    }
    out << " работает\n";
  }
}

void printRes(std::ostream & out, const std::vector< buffer > & buff, const std::vector< handler > & handlers, const std::vector< source > & sources)
{
  out << "Итоговая статистика:\n";
  int sum = 0;
  out << "Источники:\n";
  for(size_t i = 0; i < sources.size(); ++i)
  {
    out << sources[i].getAppNum() << " - " << sources[i].getName() << '\n';
    sum += sources[i].getAppNum();
  }
  out << "Суммарно было сгенерировано " << sum << " заявок\n";
  sum = 0;
  for(size_t i = 0; i < buff.size(); ++i)
  {
    sum += buff[i].getDelNum();
  }
  out << "Из-за переполнения буферов было удалено " << sum << " заявок\n";
  sum = 0;
  out << "Менеджеры:\n";
  for(size_t i = 0; i < handlers.size(); ++i)
  {
    out << (i + 1) << "й менджер обработал " << handlers[i].getAppNum() << " заявок\n";
    sum += handlers[i].getAppNum();
  }
  out << "Суммарно было обработано " << sum << " заявок\n";
}
