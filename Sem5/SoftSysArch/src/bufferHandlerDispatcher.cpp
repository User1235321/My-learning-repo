#include "bufferHandlerDispatcher.hpp"

bufferHandlerDispatcher::bufferHandlerDispatcher(std::vector< handler > & handlers, std::vector< buffer > & buff, std::ostream * out, size_t sleepTime):
  handlers_(handlers),
  buff_(buff),
  out_(out),
  sleepTime_(sleepTime),
  handlerNum_(0),
  actualBuff_(0){}

bufferHandlerDispatcher::~bufferHandlerDispatcher()
{
  stopAutoWork();
}

bufferHandlerDispatcher::bufferHandlerDispatcher(const bufferHandlerDispatcher & bhd):
  handlers_(bhd.handlers_),
  buff_(bhd.buff_),
  out_(bhd.out_),
  handlerNum_(bhd.handlerNum_),
  sleepTime_(bhd.sleepTime_){}

bufferHandlerDispatcher::bufferHandlerDispatcher(bufferHandlerDispatcher && bhd) noexcept:
  handlers_(bhd.handlers_),
  buff_(bhd.buff_),
  out_(bhd.out_),
  handlerNum_(bhd.handlerNum_),
  sleepTime_(bhd.sleepTime_){}

bufferHandlerDispatcher & bufferHandlerDispatcher::operator=(const bufferHandlerDispatcher & bhd)
{
  handlers_ = bhd.handlers_;
  buff_ = bhd.buff_;
  out_ = bhd.out_;
  handlerNum_ = bhd.handlerNum_;
  sleepTime_ = bhd.sleepTime_;
  return *this;
}
bufferHandlerDispatcher & bufferHandlerDispatcher::operator=(bufferHandlerDispatcher && bhd) noexcept
{
  handlers_ = bhd.handlers_;
  buff_ = bhd.buff_;
  out_ = bhd.out_;
  handlerNum_ = bhd.handlerNum_;
  sleepTime_ = bhd.sleepTime_;
  return *this;
}


void bufferHandlerDispatcher::stepWork()
{
  size_t startNum = handlerNum_, startBuff_ = actualBuff_;
  auto app = buff_[actualBuff_].pop();
  for (size_t i = 0; i < handlers_.size(); ++i)
  {
    if (handlers_[handlerNum_].free())
    {
      if (app.get() != nullptr)
      {
        (*app).actualStageNum_ = 4;
        (*out_) << "\033[1;35mЗаявка с id " << (*app).id_ << " обработана диспетчером выбора\033[0m\n";
        handlers_[handlerNum_].takeApp(app);
        ++handlerNum_;
        handlerNum_ %= handlers_.size();
        app = buff_[actualBuff_].pop();
        ++actualBuff_;
        actualBuff_ %= buff_.size();
      }
      else
      {
        i = handlers_.size();
      }
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

void bufferHandlerDispatcher::replaceOut(std::ostream * out)
{
  out_ = out;
}
