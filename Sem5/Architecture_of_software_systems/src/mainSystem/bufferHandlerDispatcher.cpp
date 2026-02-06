#include "bufferHandlerDispatcher.hpp"

bufferHandlerDispatcher::bufferHandlerDispatcher(const bufferHandlerDispatcher & bhd):
  actualBuff_(bhd.actualBuff_),
  handlerNum_(bhd.handlerNum_),
  sleepTime_(bhd.sleepTime_),
  handlers_(bhd.handlers_),
  buff_(bhd.buff_){}

bufferHandlerDispatcher::bufferHandlerDispatcher(bufferHandlerDispatcher && bhd):
  actualBuff_(bhd.actualBuff_),
  handlerNum_(bhd.handlerNum_),
  sleepTime_(bhd.sleepTime_),
  handlers_(bhd.handlers_),
  buff_(bhd.buff_){}

bufferHandlerDispatcher & bufferHandlerDispatcher::operator=(const bufferHandlerDispatcher & bhd)
{
  if (&bhd != this)
  {
    actualBuff_ = bhd.actualBuff_;
    handlerNum_ = bhd.handlerNum_;
    sleepTime_ = bhd.sleepTime_;
    handlers_ = bhd.handlers_;
    buff_ = bhd.buff_;
  }
  return *this;
}

bufferHandlerDispatcher & bufferHandlerDispatcher::operator=(bufferHandlerDispatcher && bhd)
{
  if (&bhd != this)
  {
    actualBuff_ = bhd.actualBuff_;
    handlerNum_ = bhd.handlerNum_;
    sleepTime_ = bhd.sleepTime_;
    handlers_ = bhd.handlers_;
    buff_ = bhd.buff_;
  }
  return *this;
}

bufferHandlerDispatcher::~bufferHandlerDispatcher()
{
  stopAutoWork();
}

bufferHandlerDispatcher::bufferHandlerDispatcher(size_t sleepTime, std::vector< handler > * handlers, std::vector< buffer > * buff):
  actualBuff_(0),
  handlerNum_(0),
  sleepTime_(sleepTime),
  handlers_(handlers),
  buff_(buff){}

void bufferHandlerDispatcher::stepWork()
{
  for (size_t i = 0; i < (handlers_->size()); ++i)
  {
    if (((*handlers_)[handlerNum_]).free())
    {
      for (size_t j = 0; j < (buff_->size()); ++j)
      {
        auto app = ((*buff_)[actualBuff_]).pop();
        if (app.id_ != 0)
        {
          ((*handlers_)[handlerNum_]).takeApp(app);
          ++handlerNum_;
          handlerNum_ %= handlers_->size();
        }
        ++actualBuff_;
        actualBuff_ %= (buff_->size());
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
    stepWork();
    std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime_));
  }
}
