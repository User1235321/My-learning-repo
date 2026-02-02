#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <atomic>
#include <chrono>
#include <thread>

#include "application.hpp"
#include "../additionalSystem/printer.hpp"

class handler
{
  public:
    handler() = default;
    handler(const handler & hand);
    handler(handler && hand);
    handler & operator=(const handler & hand);
    handler & operator=(handler && hand);
    ~handler();
    handler(size_t id, size_t sleepTime, double workDuration, printer * print);
 
    bool free() const noexcept;
    void takeApp(application app);
    void stepWork(double stepTime);
    void autoWork();
    void stopAutoWork();
    std::vector< size_t > returnPriorSucNum() const;
    std::vector< double > returnTime() const;

  private:
    size_t id_, sleepTime_;
    double workDuration_, workTime_, timeNow_;
    bool isWork_;
    application appNow_;
    std::thread thread_;
    std::atomic< bool > isRunning_{false};
    std::chrono::time_point< std::chrono::high_resolution_clock > lastWorkTime_;
    printer * print_;
    void autoWorkThread();
};

#endif
