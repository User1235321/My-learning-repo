#include "bufferHandlerDispatcher.hpp"

bufferHandlerDispatcher::bufferHandlerDispatcher(std::vector< handler > & handlers, buffer & buff, std::ostream * out, size_t sleepTime):
  handlers_(handlers),
  buff_(buff),
  out_(out),
  sleepTime_(sleepTime),
  handlerNum_(0){}

bufferHandlerDispatcher::~bufferHandlerDispatcher()
{
  stopAutoWork();
}

void bufferHandlerDispatcher::stepWork()
{
  if (handlers_[handlerNum_].free())
  {
    auto app = buff_.pop();
    if (app.get() != nullptr)
    {
      (*app).actualStageNum_ = 4;
      (*out_) << "\033[1;35mЗаявка с id " << (*app).id_ << " обработана диспетчером выбора\033[0m\n";
      handlers_[handlerNum_].takeApp(app);
      ++handlerNum_;
      handlerNum_ %= 3;
    }
  }
}

void bufferHandlerDispatcher::autoWork()
{
  if (!isRunning_)
  {
    isRunning_ = true;
    thread_ = std::thread(&bufferHandlerDispatcher::dispatcherThreadFunc, this);
  }
}

void bufferHandlerDispatcher::stopAutoWork()
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

void bufferHandlerDispatcher::dispatcherThreadFunc()
{
  while (isRunning_)
  {
    std::lock_guard< std::mutex > lock(mutex_);
    stepWork();
    std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime_));
  }
}
