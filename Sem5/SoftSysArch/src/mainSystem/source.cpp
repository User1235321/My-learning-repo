#include "source.hpp"

#include <cmath>

source::source(const source & src):
  id_(src.id_),
  priority_(src.priority_),
  sleepTime_(src.sleepTime_),
  workTime_(src.workTime_),
  lambda_(src.lambda_),
  timeToNextApp_(0),
  initTime_(src.initTime_),
  apps_(src.apps_),
  print_(src.print_)
  {
    timeToNextApp_ = -log((rand() % 10000 + 1) / 10001.0) / lambda_;
  }

source::source(source && src):
  id_(src.id_),
  priority_(src.priority_),
  sleepTime_(src.sleepTime_),
  workTime_(src.workTime_),
  lambda_(src.lambda_),
  timeToNextApp_(0),
  initTime_(src.initTime_),
  apps_(src.apps_),
  print_(src.print_)
  {
    timeToNextApp_ = -log((rand() % 10000 + 1) / 10001.0) / lambda_;
  }

source & source::operator=(const source & src)
{
  if (&src != this)
  {
    id_ = src.id_;
    priority_ = src.priority_;
    sleepTime_ = src.sleepTime_;
    workTime_ = src.workTime_;
    lambda_ = src.lambda_;
    timeToNextApp_ -log((rand() % 10000 + 1) / 10001.0) / lambda_;
    initTime_ = src.initTime_;
    apps_ = src.apps_;
    print_ = src.print_;
  }
  return *this;
}

source & source::operator=(source && src)
{
  if (&src != this)
  {
    id_ = src.id_;
    priority_ = src.priority_;
    sleepTime_ = src.sleepTime_;
    workTime_ = src.workTime_;
    lambda_ = src.lambda_;
    timeToNextApp_ -log((rand() % 10000 + 1) / 10001.0) / lambda_;
    initTime_ = src.initTime_;
    apps_ = src.apps_;
    print_ = src.print_;
  }
  return *this;
}

source::~source()
{
  stopAutoWork();
}

source::source(size_t id, size_t priority, size_t sleepTime, double lambda, printer * print):
  id_(id),
  priority_(priority),
  sleepTime_(sleepTime),
  workTime_(0),
  lambda_(lambda),
  timeToNextApp_(0),
  print_(print)
  {
    timeToNextApp_ = -log((rand() % 10000 + 1) / 10001.0) / lambda_;
  }

application source::returnApp()
{
  application app;
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

void source::autoWork()
{
  initTime_ = std::chrono::high_resolution_clock::now();
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
    print_ -> endSource(id_, std::chrono::duration< double >(std::chrono::high_resolution_clock::now() - initTime_).count(), workTime_);
    isRunning_ = false;
    if (thread_.joinable())
    {
      thread_.join();
    }
  }
}

void source::createNewApp()
{
  size_t newId = actualAppId_.fetch_add(1);
  print_ -> printSource(id_, newId, priority_);
  apps_.push(application{newId, priority_, id_, std::chrono::high_resolution_clock::now()});
}

void source::autoWorkThread()
{
  auto lastUpdate = std::chrono::high_resolution_clock::now();
  while (isRunning_)
  {
    auto now = std::chrono::high_resolution_clock::now();
    double timePassed = std::chrono::duration< double >(now - lastUpdate).count();
    if (timePassed >= timeToNextApp_) 
    {
      createNewApp();
      timeToNextApp_ = -log((rand() % 10000 + 1) / 10001.0) / lambda_;
      workTime_ += timePassed;
      lastUpdate = now;
      std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime_));
    }
  }
}
