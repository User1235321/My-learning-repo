#include "printer.hpp"

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

}

void printer::endSource(size_t id, double fullTime, double workTime)
{
  (srcStats_[id]).timeInSystem_ = fullTime;
  (srcStats_[id]).workingTime_ = workTime;
}

void printer::printSource(size_t id, size_t actualAppId, size_t priority)
{
  ++((srcStats_[id]).generates_);
  if (autoMod_)
  {
    std::lock_guard< std::mutex > lock(outMutex_);
    (*out_) << "\033[1;33mИсточник \"" << (srcStats_[id]).name_ << "\" создал заявку №" << (actualAppId + 1) << " с приоритетом " << priority << "\033[0m\n";
  }
}

void printer::printBufferIn(size_t id, std::shared_ptr< application > app)
{
  ++(buffFullness_[id]);
  if (autoMod_)
  {
    std::lock_guard< std::mutex > lock(outMutex_);
    (*out_) << "\033[1;34mЗаявка №" << ((*app).id_ + 1) << " положена в буфер №" << (id + 1) << "\033[0m\n";
  }
}

void printer::printBufferOut(size_t id)
{
  --(buffFullness_[id]);
}

void printer::printBufferDel(size_t id, std::shared_ptr< application > app)
{
  --(buffFullness_[id]);
  ++((srcStats_[(*app).sourceId_]).deleted_);
  if (autoMod_)
  {
    std::lock_guard< std::mutex > lock(outMutex_);
    (*out_) << "\033[1;31mЗаявка №" << ((*app).id_ + 1) << " удалена из буфера №" << (id + 1) << " из-за переполнения\033[0m\n";
  }
}

void printer::printHandlerIn(size_t id, std::shared_ptr< application > app)
{
  (handStats_[id]).isWorking_ = true;
  if (autoMod_)
  {
    std::lock_guard< std::mutex > lock(outMutex_);
    (*out_) << "\033[1;35mЗаявка №" << ((*app).id_ + 1) << " обрабатывается Менеджером №" << (id + 1) << "\033[0m\n";
  }
}

void printer::printHandlerOut(size_t id, std::shared_ptr< application > app)
{
  (handStats_[id]).isWorking_ = false;
  ++((handStats_[id]).sucsessfull_);
  ++((srcStats_[(*app).sourceId_]).sucsessfull_);
  if (autoMod_)
  {
    std::lock_guard< std::mutex > lock(outMutex_);
    (*out_) << "\033[1;32mЗаявка №" << ((*app).id_ + 1) << " обработана Менеджером №" << (id + 1) << "\033[0m\n";
  }
}

void printer::endHandler(size_t id, double workTime)
{
  (handStats_[id]).workingTime_ = workTime;
}
