#include "sourceBufferDispatcher.hpp"

sourceBufferDispatcher::sourceBufferDispatcher(const sourceBufferDispatcher & sbd):
  actualBuff_(sbd.actualBuff_),
  sleepTime_(sbd.sleepTime_),
  sources_(sbd.sources_),
  buff_(sbd.buff_){}

sourceBufferDispatcher::sourceBufferDispatcher(sourceBufferDispatcher && sbd):
  actualBuff_(sbd.actualBuff_),
  sleepTime_(sbd.sleepTime_),
  sources_(sbd.sources_),
  buff_(sbd.buff_){}

sourceBufferDispatcher & sourceBufferDispatcher::operator=(const sourceBufferDispatcher & sbd)
{
  if (&sbd != this)
  {
    actualBuff_ = sbd.actualBuff_;
    sleepTime_ = sbd.sleepTime_;
    sources_ = sbd.sources_;
    buff_ = sbd.buff_;
  }
  return *this;
}
sourceBufferDispatcher & sourceBufferDispatcher::operator=(sourceBufferDispatcher && sbd) 
{
  if (&sbd != this)
  {
    actualBuff_ = sbd.actualBuff_;
    sleepTime_ = sbd.sleepTime_;
    sources_ = sbd.sources_;
    buff_ = sbd.buff_;
  }
  return *this;
}

sourceBufferDispatcher::sourceBufferDispatcher(size_t sleepTime, std::vector< source > * sources, std::vector< buffer > * buff):
  actualBuff_(0),
  sleepTime_(sleepTime),
  sources_(sources),
  buff_(buff){}

sourceBufferDispatcher::~sourceBufferDispatcher()
{
  stopAutoWork();
}

void sourceBufferDispatcher::stepWork()
{
  for(auto & src : (*sources_))
  {
    auto app = src.returnApp();
    if (app.id_ != 0)
    {
      ((*buff_)[actualBuff_]).push(app);
      ++actualBuff_;
      actualBuff_ %= (buff_->size());
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
