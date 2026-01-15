#include "sourceBufferDispatcher.hpp"

sourceBufferDispatcher::sourceBufferDispatcher(std::vector< source > & sources, buffer & buff, std::ostream * out, size_t sleepTime):
  sources_(sources),
  buff_(buff),
  out_(out),
  sleepTime_(sleepTime){}

sourceBufferDispatcher::~sourceBufferDispatcher()
{
  stopAutoWork();
}

void sourceBufferDispatcher::stepWork()
{
  for(auto & src : sources_)
  {
    auto app = src.returnApp();
    if (app.get() != nullptr)
    {
      (*app).actualStageNum_ = 2;
      (*out_) << "\033[1;32mЗаявка с id " << (*app).id_ << " обработана диспетчером постановки\033[0m\n";
      buff_.push(app);
    }
  }
}

void sourceBufferDispatcher::autoWork()
{
  isRunning_ = true;
  dispatcherThread_ = std::thread(&sourceBufferDispatcher::dispatcherThreadFunc, this);
}

void sourceBufferDispatcher::stopAutoWork()
{
  if (isRunning_)
  {
    isRunning_ = false;
    if (dispatcherThread_.joinable())
    {
      dispatcherThread_.join();
    }
  }
}

void sourceBufferDispatcher::dispatcherThreadFunc()
{
  while (isRunning_)
  {
    std::lock_guard< std::mutex > lock(workMutex_);
    stepWork();
    std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime_));
  }
}
