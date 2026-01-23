#ifndef SOURCE_HPP
#define SOURCE_HPP

#include <memory>
#include <iostream>
#include <string>
#include <queue>
#include <chrono>
#include <thread>
#include <atomic>
#include <mutex>

#include "application.hpp"

class source
{
  public:
    source() = default;
    source(const source & src);
    source(source && src);
    source & operator=(const source & src);
    source & operator=(source && src);
    ~source();
    source(int priority, std::string sourceName, double lambda, std::ostream * out, size_t sleepTime, std::mutex * outMutex);

    std::shared_ptr< application > returnApp();
    void stepWork(double stepTime);
    void autoWork();
    void stopAutoWork();
    size_t getAppNum() const noexcept;
    std::string getName() const;
    void replaceOut(std::ostream * out);

  private:
    int priority_;
    static inline int id_;
    double lambda_, timeToNextApp_;
    size_t sleepTime_, appNum_;
    std::string sourceName_;
    std::queue< std::shared_ptr< application > > apps_;
    std::ostream * out_;
    std::thread thread_;
    std::atomic< bool > isRunning_{false};
    std::mutex * outMutex_;
    std::chrono::time_point< std::chrono::high_resolution_clock > lastWorkTime_;
    void createNewApp();
    void autoWorkThread();
};

#endif
