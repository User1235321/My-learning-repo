#include "handler.hpp"


handler::handler(const handler & hand):
  id_(hand.id_),
  sleepTime_(hand.sleepTime_),
  workDuration_(hand.workDuration_),
  workTime_(hand.workTime_),
  timeNow_(hand.timeNow_),
  isWork_(hand.isWork_),
  appNow_(hand.appNow_),
  print_(hand.print_){}

handler::handler(handler && hand):
  id_(hand.id_),
  sleepTime_(hand.sleepTime_),
  workDuration_(hand.workDuration_),
  workTime_(hand.workTime_),
  timeNow_(hand.timeNow_),
  isWork_(hand.isWork_),
  appNow_(hand.appNow_),
  print_(hand.print_){}

handler & handler::operator=(const handler & hand)
{
  if (&hand != this)
  {
    id_ = hand.id_;
    sleepTime_ = hand.sleepTime_;
    workDuration_ = hand.workDuration_;
    workTime_ = hand.workTime_;
    timeNow_ = hand.timeNow_;
    isWork_ = hand.isWork_;
    appNow_ = hand.appNow_;
    print_ = hand.print_;
  }
  return *this;
}

handler & handler::operator=(handler && hand)
{
  if (&hand != this)
  {
    id_ = hand.id_;
    sleepTime_ = hand.sleepTime_;
    workDuration_ = hand.workDuration_;
    workTime_ = hand.workTime_;
    timeNow_ = hand.timeNow_;
    isWork_ = hand.isWork_;
    appNow_ = hand.appNow_;
    print_ = hand.print_;
  }
  return *this;
}

handler::~handler()
{
  stopAutoWork();
}

handler::handler(size_t id, size_t sleepTime, double workDuration, printer * print):
  id_(id),
  sleepTime_(sleepTime),
  workDuration_(workDuration),
  workTime_(0),
  timeNow_(0),
  isWork_(false),
  print_(print){}

bool handler::free() const noexcept
{
  return !isWork_;
}

void handler::takeApp(application app)
{
  appNow_ = app;
  isWork_ = true;
  lastWorkTime_ = std::chrono::high_resolution_clock::now();
  timeNow_ = 0.0;
  print_->printHandlerIn(id_, app);
}

void handler::stepWork(double stepTime)
{
  if (isWork_ && (appNow_.id_ != 0))
  {
    timeNow_ += stepTime;
    if (timeNow_ >= workDuration_)
    {
      print_->printHandlerOut(id_, appNow_);
      appNow_ = application{0, 0, 0, std::chrono::time_point< std::chrono::high_resolution_clock >()};
      isWork_ = false;
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
    print_->endHandler(id_, workTime_);
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
    if (isWork_ && (appNow_.id_ != 0))
    {
      auto now = std::chrono::high_resolution_clock::now();
      double timePassed = std::chrono::duration< double >(now - lastWorkTime_).count();
      if (timePassed >= workDuration_)
      {
        print_ -> printHandlerOut(id_, appNow_);
        appNow_ = application{0, 0, 0, std::chrono::time_point< std::chrono::high_resolution_clock >()};
        isWork_ = false;
        workTime_ += timePassed;
      }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime_));
  }
}
