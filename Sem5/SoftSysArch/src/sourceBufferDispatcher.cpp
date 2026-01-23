#include "sourceBufferDispatcher.hpp"

sourceBufferDispatcher::sourceBufferDispatcher(std::vector< source > & sources, std::vector< buffer > & buff, std::ostream * out, size_t sleepTime, std::mutex * outMutex):
  sleepTime_(sleepTime),
  actualBuff_(0),
  sources_(sources),
  buff_(buff),
  out_(out),
  outMutex_(outMutex){}

sourceBufferDispatcher::~sourceBufferDispatcher()
{
  stopAutoWork();
}

sourceBufferDispatcher::sourceBufferDispatcher(const sourceBufferDispatcher & sbd):
  sleepTime_(sbd.sleepTime_),
  actualBuff_(sbd.actualBuff_),
  sources_(sbd.sources_),
  buff_(sbd.buff_),
  out_(sbd.out_),
  outMutex_(sbd.outMutex_){}

sourceBufferDispatcher::sourceBufferDispatcher(sourceBufferDispatcher && sbd) noexcept:
  sleepTime_(sbd.sleepTime_),
  actualBuff_(sbd.actualBuff_),
  sources_(sbd.sources_),
  buff_(sbd.buff_),
  out_(sbd.out_),
  outMutex_(sbd.outMutex_){}

sourceBufferDispatcher & sourceBufferDispatcher::operator=(const sourceBufferDispatcher & sbd)
{
  sleepTime_ = sbd.sleepTime_;
  actualBuff_ = sbd.actualBuff_;
  sources_ = sbd.sources_;
  buff_ = sbd.buff_;
  out_ = sbd.out_;
  outMutex_ = sbd.outMutex_;
  return *this;
}
sourceBufferDispatcher & sourceBufferDispatcher::operator=(sourceBufferDispatcher && sbd) noexcept
{
  sleepTime_ = sbd.sleepTime_;
  actualBuff_ = sbd.actualBuff_;
  sources_ = sbd.sources_;
  buff_ = sbd.buff_;
  out_ = sbd.out_;
  outMutex_ = sbd.outMutex_;
  return *this;
}

void sourceBufferDispatcher::stepWork()
{
  for(auto & src : sources_)
  {
    auto app = src.returnApp();
    if (app.get() != nullptr)
    {
      (*app).actualStageNum_ = 2;
      {
        std::lock_guard< std::mutex > lock(*outMutex_);
        (*out_) << "\033[1;32mЗаявка с id " << (*app).id_ << " обработана диспетчером постановки\033[0m\n";
      }
      buff_[actualBuff_].push(app);
      ++actualBuff_;
      actualBuff_ %= buff_.size();
    }
  }
}

void sourceBufferDispatcher::autoWork()
{
  if (!isRunning_)
  {
    isRunning_ = true;
    thread_ = std::thread(&sourceBufferDispatcher::dispatcherThreadFunc, this);
  }
}

void sourceBufferDispatcher::stopAutoWork()
{
  if (isRunning_)
  {
    isRunning_ = false;
    if (thread_.joinable())
    {
      thread_.join();
    }
  }
}

void sourceBufferDispatcher::dispatcherThreadFunc()
{
  while (isRunning_)
  {
    stepWork();
    std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime_));
  }
}

void sourceBufferDispatcher::replaceOut(std::ostream * out)
{
  out_ = out;
}
