#include "source.hpp"

#include <cmath>

source::source(const source & src):
  priority_(src.priority_),
  sourceName_(src.sourceName_),
  lambda_(src.lambda_),
  timeToNextApp_(0),
  apps_(src.apps_),
  out_(src.out_),
  sleepTime_(src.sleepTime_)
  {
    timeToNextApp_ = -log((rand() % 10000 + 1) / 10001.0) / lambda_;
  }

source::source(source && src):
  priority_(src.priority_),
  sourceName_(std::move(src.sourceName_)),
  lambda_(src.lambda_),
  timeToNextApp_(0),
  apps_(std::move(src.apps_)),
  out_(src.out_),
  sleepTime_(src.sleepTime_)
  {
    timeToNextApp_ = -log((rand() % 10000 + 1) / 10001.0) / lambda_;
  }

source & source::operator=(const source & src)
{
  priority_ = src.priority_;
  sourceName_ = src.sourceName_;
  lambda_ = src.lambda_;
  timeToNextApp_ = -log((rand() % 10000 + 1) / 10001.0) / lambda_;
  apps_ = src.apps_;
  out_ = src.out_;
  sleepTime_ = src.sleepTime_;
  return *this;
}

source & source::operator=(source && src)
{
  priority_ = src.priority_;
  sourceName_ = std::move(src.sourceName_);
  lambda_ = src.lambda_;
  timeToNextApp_ = -log((rand() % 10000 + 1) / 10001.0) / lambda_;
  apps_ = std::move(src.apps_);
  out_ = src.out_;
  sleepTime_ = src.sleepTime_;
  return *this;
}

source::~source()
{
  stopAutoWork();
}

source::source(int priority, std::string sourceName, double lambda, std::ostream * out, size_t sleepTime):
  priority_(priority),
  sourceName_(sourceName),
  lambda_(lambda),
  timeToNextApp_(0),
  out_(out),
  sleepTime_(sleepTime),
  appNum_(0)
  {
    timeToNextApp_ = -log((rand() % 10000 + 1) / 10001.0) / lambda_;
  }

void source::createNewApp()
{
  std::lock_guard< std::mutex > lock(mutex_);
  (*out_) << "\033[1;31mИсточник заявок " << sourceName_ << " создал заявку с id " << ++id_ << " и приоритетом " << priority_ << "\033[0m\n";
  apps_.push(std::make_unique< application >(application{priority_, id_, 1}));
  ++appNum_;
}

std::shared_ptr< application > source::returnApp()
{
  std::shared_ptr< application > app;
  if (!apps_.empty())
  {
    app = apps_.front();
    apps_.pop();
  }

  return app;
}

void source::stepWork(double stepTime)
{
  timeToNextApp_ -= stepTime;
  if (timeToNextApp_ <= 0)
  {
    createNewApp();
    timeToNextApp_ = -log((rand() % 10000 + 1) / 10001.0) / lambda_;
  }
}

void source::autoWorkThread()
{
  auto lastUpdate = std::chrono::high_resolution_clock::now();
  while (isRunning_)
  {
    auto now = std::chrono::high_resolution_clock::now();
    timeToNextApp_ -= std::chrono::duration< double >(now - lastUpdate).count();
    if (timeToNextApp_ <= 0)
    {
      createNewApp();
      timeToNextApp_ = -log((rand() % 10000 + 1) / 10001.0) / lambda_;
    }
    lastUpdate = now;
    std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime_));
  }
}

void source::autoWork()
{
  if (!isRunning_)
  {
    isRunning_ = true;
    thread_ = std::thread(&source::autoWorkThread, this);
  }
}

void source::stopAutoWork()
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

size_t source::getAppNum() const noexcept
{
    return appNum_;
}

std::string source::getName() const
{
  return sourceName_;
}

void source::replaceOut(std::ostream * out)
{
  out_ = out;
}
