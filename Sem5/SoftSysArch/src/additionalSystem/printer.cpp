#include "printer.hpp"

#include <chrono>
#include <cmath>
#include <codecvt>
#include <iomanip>

printer::printer(size_t numOfSrc, const std::vector< std::string > & sourcesNames, size_t numOfBuffs, size_t numOfHands, size_t buffSize, std::ostream * out):
  buffSize_(buffSize),
  autoMod_(false),
  out_(out)
  {
    for (size_t i = 0; i < numOfSrc; ++i)
    {
      srcStats_.push_back(sourceStats{0, 0, 0, 0, 0, sourcesNames[i], std::vector< long double >(0)});
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

void printer::printStep(size_t step, long double totalTime)
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

void addSpaces(std::vector< std::vector< std::string > > & vec)
{
  std::wstring_convert< std::codecvt_utf8< wchar_t > > converter;
  for (size_t i = 0; i < vec[0].size(); ++i)
  {
    size_t width = 0;
    for (size_t j = 0; j < vec.size(); ++j)
    {
      width = std::max(width, (converter.from_bytes(vec[j][i])).length());
    }
    width += 5;
    for (size_t j = 0; j < vec.size(); ++j)
    {
      vec[j][i] += std::string(width - (converter.from_bytes(vec[j][i])).length(), ' ');
    }
  }
}

void printer::printRes(long double totalTime)
{
  long double tt = totalTime;
  for (auto & src : srcStats_)
  {
    tt = std::max(tt, src.timeInSystem_);
  }
  for (auto & hand : handStats_)
  {
    tt = std::max(tt, hand.workingTime_);
  }
  std::lock_guard< std::mutex > lock(outMutex_);
  (*out_) << "Общее время работы системы: " << std::fixed << std::setprecision(4) << tt << '\n';
  std::vector< std::vector< std::string > > srcOut;
  srcOut.push_back(std::vector< std::string >{"Источник", "Сгенерировано", "Обработано", "Удалено", "В системе", "Время работы", "Время простоя", "Мат. ожидание", "Дисперсия"});
  for (size_t i = 0; i < srcStats_.size(); ++i)
  {
    long double sum = 0;
    for (long double stats : ((srcStats_[i]).appsTimes_))
    {
      sum += stats;
    }
    long double count = (srcStats_[i].appsTimes_).size();
    long double mid = sum / count;
    sum = 0;
    for (long double stats : ((srcStats_[i]).appsTimes_))
    {
      sum += std::pow(stats - mid, 2);
    }
    long double disp = sum / count;
    srcOut.push_back(std::vector< std::string >{srcStats_[i].name_, std::to_string(srcStats_[i].generates_), std::to_string(srcStats_[i].sucsessfull_), std::to_string(srcStats_[i].deleted_), std::to_string(srcStats_[i].timeInSystem_), std::to_string(srcStats_[i].workingTime_), std::to_string(srcStats_[i].timeInSystem_ - srcStats_[i].workingTime_), std::to_string(mid), std::to_string(disp)});
  }
  addSpaces(srcOut);
  (*out_) << '\n';
  for (auto & str : srcOut)
  {
    for (auto & word : str)
    {
      (*out_) << word;
    }
    (*out_) << '\n';
  }
  (*out_) << "\n\n";
  std::vector< std::vector< std::string > > handOut;
  handOut.push_back(std::vector< std::string >{"Менеджер", "Обработано", "Время работы", "Загруженность"});
  for (size_t i = 0; i < handStats_.size(); ++i)
  {
    handOut.push_back(std::vector< std::string >{std::to_string(i), std::to_string((handStats_[i]).sucsessfull_), std::to_string((handStats_[i]).workingTime_), std::to_string((handStats_[i]).workingTime_ * 100.0 / tt) + std::string{'%'}});
  }
  addSpaces(handOut);
  for (auto & str : handOut)
  {
    for (auto & word : str)
    {
      (*out_) << word;
    }
    (*out_) << '\n';
  }
}

void printer::endSource(size_t id, long double fullTime, long double workTime)
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
    ((srcStats_[app.sourceId_]).appsTimes_).push_back(std::chrono::duration< long double >(std::chrono::high_resolution_clock::now() - app.startTime_).count());
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
    ((srcStats_[app.sourceId_]).appsTimes_).push_back(std::chrono::duration< long double >(std::chrono::high_resolution_clock::now() - app.startTime_).count());
  }
  if (autoMod_)
  {
    std::lock_guard< std::mutex > lock2(outMutex_);
    (*out_) << "\033[1;32mЗаявка №" << (app.id_ + 1) << " обработана Менеджером №" << (id + 1) << "\033[0m\n";
  }
}

void printer::endHandler(size_t id, long double workTime)
{
  std::lock_guard< std::mutex > lock(handMutex_);
  (handStats_[id]).workingTime_ = workTime;
}
