#include "handler.hpp"

handler::~handler()
{
  stopAutoWork();
}

handler::handler(std::ostream * out, size_t sleepTime, double workTime):
  out_(out),
  sleepTime_(sleepTime),
  workTime_(workTime),
  isWork_(false),
  timeNow_(0),
  appNum_(0){}

handler::handler(const handler & hand):
  appNow_(hand.appNow_),
  out_(hand.out_),
  isWork_(hand.isWork_),
  sleepTime_(hand.sleepTime_),
  workTime_(hand.workTime_){}

handler::handler(handler && hand):
  appNow_(hand.appNow_),
  out_(hand.out_),
  isWork_(hand.isWork_),
  sleepTime_(hand.sleepTime_),
  workTime_(hand.workTime_){}

handler & handler::operator=(const handler & hand)
{
  appNow_ = hand.appNow_;
  out_ = hand.out_;
  isWork_ = hand.isWork_;
  sleepTime_ = hand.sleepTime_;
  workTime_ = hand.workTime_;
  return *this;
}

handler & handler::operator=(handler && hand)
{
  appNow_ = hand.appNow_;
  out_ = hand.out_;
  isWork_ = hand.isWork_;
  sleepTime_ = hand.sleepTime_;
  workTime_ = hand.workTime_;
  return *this;
}

bool handler::free()
{
  return !isWork_;
}

void handler::takeApp(std::shared_ptr< application > app)
{
  appNow_ = app;
  isWork_ = true;
  lastWorkTime_ = std::chrono::high_resolution_clock::now();
  timeNow_ = 0.0;
  (*out_) << "Менеджер начинает работу\n";
}

void handler::stepWork(double stepTime)
{
  if (isWork_)
  {
    timeNow_ += stepTime;
    if (timeNow_ >= workTime_)
    {
      (*out_) << "\033[1;36mЗаявка с id " << (*appNow_).id_ << " обработана менеджером\033[0m\n";
      appNow_ = nullptr;
      isWork_ = false;
      ++appNum_;
    }
  }
}

void handler::autoWork()
{
  if (!isRunning_)
  {
    isRunning_ = true;
    thread_ = std::thread(&handler::autoWorkThread, this);
  }
}

void handler::stopAutoWork()
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

void handler::autoWorkThread()
{
  while (isRunning_)
  {
    std::lock_guard< std::mutex > lock(mutex_);
    if (isWork_)
    {
      auto now = std::chrono::high_resolution_clock::now();
      if (std::chrono::duration< double >(now - lastWorkTime_).count() >= workTime_)
      {
        (*out_) << "\033[1;36mЗаявка с id " << (*appNow_).id_ << " обработана менеджером\033[0m\n";
        appNow_ = nullptr;
        isWork_ = false;
        ++appNum_;
      }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime_));
  }
}

size_t handler::getAppNum()
{
  return appNum_;
}
