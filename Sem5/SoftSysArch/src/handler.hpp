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
    handler(std::ostream * out, size_t sleepTime, double workTime, size_t priorNum, std::mutex * outMutex);
    handler(const handler & hand);
    handler(handler && hand);
    handler & operator=(const handler & hand);
    handler & operator=(handler && hand);

    bool free() const noexcept;
    void takeApp(std::shared_ptr< application > app);
    void stepWork(double stepTime);
    void autoWork();
    void stopAutoWork();
    size_t getAppNum() const noexcept;
    void replaceOut(std::ostream * out);
    std::vector< size_t > returnPriorSucNum() const;
    std::vector< double > returnTime() const;

  private:
    size_t sleepTime_, appNum_;
    double workTime_, timeNow_;
    bool isWork_;
    std::vector< size_t > priorSucNum_;
    std::vector< double > times_;
    std::shared_ptr< application > appNow_;
    std::ostream * out_;
    std::thread thread_;
    std::atomic< bool > isRunning_{false};
    std::mutex * outMutex_;
    std::chrono::time_point< std::chrono::high_resolution_clock > lastWorkTime_;
    void autoWorkThread();
};

#endif
