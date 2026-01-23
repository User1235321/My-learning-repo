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
  std::vector< double > times(buff[0].returnTime());
  out << "Итоговая статистика:\n";
  int sum1 = 0;
  out << "Источники:\n";
  std::vector< size_t > priorDelNum(buff[0].returnPriorDelNum());
  for (size_t i = 1; i < buff.size(); ++i)
  {
    auto t = buff[i].returnTime();
    times.insert(times.end(), t.begin(), t.end());
    std::vector< size_t > priorDelNumNow(buff[i].returnPriorDelNum());
    for (size_t j = 0; j < sources.size(); ++j)
    {
        priorDelNum[j] += priorDelNumNow[j];
    }
  }
  std::vector< size_t > priorSucNum(handlers[0].returnPriorSucNum());
  auto tk = handlers[0].returnTime();
  times.insert(times.end(), tk.begin(), tk.end());
  for (size_t i = 1; i < handlers.size(); ++i)
  {
    auto t = handlers[i].returnTime();
    times.insert(times.end(), t.begin(), t.end());
    std::vector< size_t > priorSucNumNow(handlers[i].returnPriorSucNum());
    for (size_t j = 0; j < sources.size(); ++j)
    {
        priorSucNum[j] += priorSucNumNow[j];
    }
  }
  for (size_t i = 0; i < sources.size(); ++i)
  {
    out << sources[i].getName() << " сгенерировано " << sources[i].getAppNum() << ", из них удалено " << priorDelNum[i] << ", успешно обработано " << priorSucNum[i] << '\n';
    sum1 += sources[i].getAppNum();
  }
  out << "Суммарно было сгенерировано " << sum1 << " заявок\n";
  int sum = 0;
  for (size_t i = 0; i < buff.size(); ++i)
  {
    sum += buff[i].getDelNum();
  }
  out << "Из-за переполнения буферов было удалено " << sum << " заявок\n";
  sum = 0;
  out << "Менеджеры:\n";
  for (size_t i = 0; i < handlers.size(); ++i)
  {
    sum += handlers[i].getAppNum();
  }
  for (size_t i = 0; i < handlers.size(); ++i)
  {
    out << (i + 1) << "й менеджер обработал " << handlers[i].getAppNum() << " заявок\n";
  }
  out << "Суммарно было обработано " << sum << " заявок. Загруженность: " << ((sum * 100) / sum1) << "%\n";
  long long matOpt = 0, matOpt2 = 0, disp = 0;
  for (size_t i = 0; i < sources.size(); ++i)
  {
    matOpt += sources[i].getAppNum() * (i + 1);
    matOpt2 += sources[i].getAppNum() * (i + 1) * (i + 1);
  }
  disp = matOpt2 - (matOpt * matOpt);
  out << "Математическое ожидание: " << matOpt << ", дисперсия: " << disp << '\n';
  double srTime = 0;
  for (double t : times)
  {
    srTime += t;
  }
  srTime /= times.size();
  std::cout << "Среднее время прибывания заявки в ситеме: " << srTime << '\n';
}
