#include "handler.hpp"

handler::~handler()
{
  stopAutoWork();
}

handler::handler(std::ostream * out, size_t sleepTime, double workTime, size_t priorNum, std::mutex * outMutex):
  sleepTime_(sleepTime),
  appNum_(0),
  workTime_(workTime),
  timeNow_(0),
  isWork_(false),
  out_(out),
  outMutex_(outMutex)
  {
    for (size_t i = 0; i < priorNum; ++i)
    {
      priorSucNum_.push_back(0);
    }
  }

handler::handler(const handler & hand):
  sleepTime_(hand.sleepTime_),
  appNow_(hand.appNow_),
  workTime_(hand.workTime_),
  timeNow_(hand.timeNow_),
  isWork_(hand.isWork_),
  priorSucNum_(hand.priorSucNum_),
  times_(hand.times_),
  out_(hand.out_),
  outMutex_(hand.outMutex_){}

handler::handler(handler && hand):
  sleepTime_(hand.sleepTime_),
  appNow_(hand.appNow_),
  workTime_(hand.workTime_),
  timeNow_(hand.timeNow_),
  isWork_(hand.isWork_),
  priorSucNum_(std::move(hand.priorSucNum_)),
  times_(std::move(hand.times_)),
  out_(hand.out_),
  outMutex_(hand.outMutex_){}

handler & handler::operator=(const handler & hand)
{
  sleepTime_ = hand.sleepTime_;
  appNow_ = hand.appNow_;
  workTime_ = hand.workTime_;
  timeNow_ = hand.timeNow_;
  isWork_ = hand.isWork_;
  priorSucNum_ = hand.priorSucNum_;
  times_ = hand.times_;
  out_ = hand.out_;
  outMutex_ = hand.outMutex_;
  return *this;
}

handler & handler::operator=(handler && hand)
{
  sleepTime_ = hand.sleepTime_;
  appNow_ = hand.appNow_;
  workTime_ = hand.workTime_;
  timeNow_ = hand.timeNow_;
  isWork_ = hand.isWork_;
  priorSucNum_ = std::move(hand.priorSucNum_);
  times_ = std::move(hand.times_);
  out_ = hand.out_;
  outMutex_ = hand.outMutex_;
  return *this;
}

bool handler::free() const noexcept
{
  return !isWork_;
}

void handler::takeApp(std::shared_ptr< application > app)
{
  appNow_ = app;
  isWork_ = true;
  lastWorkTime_ = std::chrono::high_resolution_clock::now();
  timeNow_ = 0.0;
}

void handler::stepWork(double stepTime)
{
  if (isWork_)
  {
    timeNow_ += stepTime;
    if (timeNow_ >= workTime_)
    {
      (*out_) << "\033[1;36mЗаявка с id " << (*appNow_).id_ << " обработана менеджером\033[0m\n";
      ++(priorSucNum_[(*appNow_).priority_ - 1]);
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
    if (isWork_)
    {
      auto now = std::chrono::high_resolution_clock::now();
      if (std::chrono::duration< double >(now - lastWorkTime_).count() >= workTime_)
      {
        {
          std::lock_guard< std::mutex > lock(*outMutex_);
          (*out_) << "\033[1;36mЗаявка с id " << (*appNow_).id_ << " обработана менеджером\033[0m\n";
        }
        times_.push_back(std::chrono::duration< double >(std::chrono::high_resolution_clock::now() - (*appNow_).startTime_).count());
        ++(priorSucNum_[(*appNow_).priority_ - 1]);
        appNow_ = nullptr;
        isWork_ = false;
        ++appNum_;
      }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime_));
  }
}

size_t handler::getAppNum() const noexcept
{
  return appNum_;
}

void handler::replaceOut(std::ostream * out)
{
  out_ = out;
}

std::vector< size_t > handler::returnPriorSucNum() const
{
  return priorSucNum_;
}

std::vector< double > handler::returnTime() const
{
  return times_;
}
