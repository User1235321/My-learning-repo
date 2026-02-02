#include "printer.hpp"

#include <chrono>
#include <cmath>
#include <iomanip>

printer::printer(size_t numOfSrc, const std::vector< std::string > & sourcesNames, size_t numOfBuffs, size_t numOfHands, size_t buffSize, std::ostream * out):
  buffSize_(buffSize),
  autoMod_(false),
  out_(out)
  {
    for (size_t i = 0; i < numOfSrc; ++i)
    {
      srcStats_.push_back(sourceStats{0, 0, 0, 0, 0, sourcesNames[i], std::vector< double >(0)});
    }
    for (size_t i = 0; i < numOfBuffs; ++i)
    {
      buffFullness_.emplace_back(0);
    }
    for (size_t i = 0; i < numOfHands; ++i)
    {
      handStats_.push_back(handlerStats{0, 0.0, false});
    }
  }

void printer::setMod(bool autoMod)
{
  autoMod_ = autoMod;
}

void printer::printStep(size_t step, double totalTime)
{
  (*out_) << "Время: " << totalTime << '\n';
  (*out_) << "Шаг " << step << '\n';
  for (size_t i = 0; i < buffFullness_.size(); ++i)
  {
    (*out_) << "В буфере " << (i + 1) << " занято " << buffFullness_[i] << " ячеек из " << buffSize_ << '\n';
  }
  for (size_t i = 0; i < handStats_.size(); ++i)
  {
    (*out_) << "Менеджер №" << (i + 1) << ' ';
    if (!handStats_[i].isWorking_)
    {
      (*out_) << "не ";
    }
    (*out_) << "работает\n";
  }
}

void printer::printRes(double totalTime)
{
  (*out_) << "Общее время работы системы: " << std::fixed << std::setprecision(4) << totalTime << '\n';
  std::vector< double > mid(srcStats_.size()), disp(srcStats_.size());
  for (size_t i = 0; i < srcStats_.size(); ++i)
  {
    double sum = 0, count = (srcStats_[i].appsTimes_).size();
    for (double stats : ((srcStats_[i]).appsTimes_))
    {
      sum += stats;
    }
    mid[i] = sum / count;
    sum = 0;
    for (double stats : ((srcStats_[i]).appsTimes_))
    {
      sum += std::pow(stats - mid[i], 2);
    }
    disp[i] = sum / count;
  }
  std::lock_guard< std::mutex > lock(outMutex_);
  const size_t width = 25;
  (*out_) << std::setw(width) << "Источник" << std::setw(width) << "Сгенерировано" << std::setw(width) << "Обработано" << std::setw(width) << "Удалено" << std::setw(width) << "В системе" << std::setw(width) << "Время работы" << std::setw(width) << "Время простоя" << std::setw(width) << "Мат. ожидание" << std::setw(width) << "Дисперсия\n";

  for (size_t i = 0; i < srcStats_.size(); ++i)
  {
    (*out_) << std::setw(width) << (srcStats_[i]).name_;
    (*out_) << std::setw(width) << (srcStats_[i]).generates_;
    (*out_) << std::setw(width) << (srcStats_[i]).sucsessfull_;
    (*out_) << std::setw(width) << (srcStats_[i]).deleted_;
    (*out_) << std::setw(width) << (srcStats_[i]).timeInSystem_;
    (*out_) << std::setw(width) << (srcStats_[i]).workingTime_;
    (*out_) << std::setw(width) << ((srcStats_[i]).timeInSystem_ - (srcStats_[i]).workingTime_);
    (*out_) << std::setw(width) << mid[i];
    (*out_) << std::setw(width) << disp[i] << '\n';
  }
  (*out_) << '\n' << std::setw(width) << "Менеджер" << std::setw(width) << "Обработано" << std::setw(width) << "Время работы" << std::setw(width) << "Загруженность\n";
  for (size_t i = 0; i < handStats_.size(); ++i)
  {
    (*out_) << std::setw(width) << i;
    (*out_) << std::setw(width) << (handStats_[i]).sucsessfull_;
    (*out_) << std::setw(width) << (handStats_[i]).workingTime_;
    (*out_) << std::setw(width) << (handStats_[i]).workingTime_ * 100.0 / totalTime << "%\n";
  }
}

void printer::endSource(size_t id, double fullTime, double workTime)
{
  std::lock_guard< std::mutex > lock(srcMutex_);
  (srcStats_[id]).timeInSystem_ = fullTime;
  (srcStats_[id]).workingTime_ = workTime;
}

void printer::printSource(size_t id, size_t actualAppId, size_t priority)
{
  {
    std::lock_guard< std::mutex > lock(srcMutex_);
    ++((srcStats_[id]).generates_);
  }
  if (autoMod_)
  {
    std::lock_guard< std::mutex > lock2(outMutex_);
    (*out_) << "\033[1;33mИсточник \"" << (srcStats_[id]).name_ << "\" создал заявку №" << (actualAppId + 1) << " с приоритетом " << priority << "\033[0m\n";
  }
}

void printer::printBufferIn(size_t id, application app)
{
  {
    std::lock_guard< std::mutex > lock(buffMutex_);
    ++(buffFullness_[id]);
  }
  if (autoMod_)
  {
    std::lock_guard< std::mutex > lock2(outMutex_);
    (*out_) << "\033[1;34mЗаявка №" << (app.id_ + 1) << " положена в буфер №" << (id + 1) << "\033[0m\n";
  }
}

void printer::printBufferOut(size_t id)
{
  std::lock_guard< std::mutex > lock(buffMutex_);
  --(buffFullness_[id]);
}

void printer::printBufferDel(size_t id, application app)
{
  {
    std::lock_guard< std::mutex > lock(buffMutex_);
    --(buffFullness_[id]);
  }
  {
    std::lock_guard< std::mutex > lock(srcMutex_);
    ++((srcStats_[app.sourceId_]).deleted_);
    ((srcStats_[app.sourceId_]).appsTimes_).push_back(std::chrono::duration< double >(std::chrono::high_resolution_clock::now() - app.startTime_).count());
  }
  if (autoMod_)
  {
    std::lock_guard< std::mutex > lock2(outMutex_);
    (*out_) << "\033[1;31mЗаявка №" << (app.id_ + 1) << " удалена из буфера №" << (id + 1) << " из-за переполнения\033[0m\n";
  }
}

void printer::printHandlerIn(size_t id, application app)
{
  {
    std::lock_guard< std::mutex > lock(handMutex_);
    (handStats_[id]).isWorking_ = true;
  }
  if (autoMod_)
  {
    std::lock_guard< std::mutex > lock2(outMutex_);
    (*out_) << "\033[1;35mЗаявка №" << (app.id_ + 1) << " обрабатывается Менеджером №" << (id + 1) << "\033[0m\n";
  }
}

void printer::printHandlerOut(size_t id, application app)
{
  {
    std::lock_guard< std::mutex > lock(handMutex_);
    (handStats_[id]).isWorking_ = false;
    ++((handStats_[id]).sucsessfull_);
  }
  {
    std::lock_guard< std::mutex > lock(srcMutex_);
    ++((srcStats_[app.sourceId_]).sucsessfull_);
    ((srcStats_[app.sourceId_]).appsTimes_).push_back(std::chrono::duration< double >(std::chrono::high_resolution_clock::now() - app.startTime_).count());
  }
  if (autoMod_)
  {
    std::lock_guard< std::mutex > lock2(outMutex_);
    (*out_) << "\033[1;32mЗаявка №" << (app.id_ + 1) << " обработана Менеджером №" << (id + 1) << "\033[0m\n";
  }
}

void printer::endHandler(size_t id, double workTime)
{
  std::lock_guard< std::mutex > lock(handMutex_);
  (handStats_[id]).workingTime_ = workTime;
}
