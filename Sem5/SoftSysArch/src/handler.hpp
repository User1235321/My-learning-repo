#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <memory>
#include <vector>
#include <atomic>
#include <mutex>
#include <thread>
#include <chrono>
#include <iostream>

#include "application.hpp"

class handler
{
  public:
  ~handler();
  handler(std::ostream * out, size_t sleepTime, double workTime);

  handler(const handler & hand);
  handler(handler && hand);
  handler & operator=(const handler & hand);
  handler & operator=(handler && hand);

  bool free();
  void takeApp(std::shared_ptr< application > app);

  void stepWork(double stepTime);
  void autoWork();
  void stopAutoWork();

  size_t getAppNum();

  private:
  std::shared_ptr< application > appNow_;
  std::ostream * out_;
  bool isWork_;
  size_t sleepTime_;
  double workTime_, timeNow_;

  std::thread thread_;
  std::atomic< bool > isRunning_{false};
  std::mutex mutex_;
  std::chrono::time_point< std::chrono::high_resolution_clock > lastWorkTime_;

  void autoWorkThread();

  size_t appNum_;
};

#endif
