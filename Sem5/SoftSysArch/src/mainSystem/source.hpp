#ifndef SOURCE_HPP
#define SOURCE_HPP

#include <atomic>
#include <chrono>
#include <memory>
#include <queue>
#include <string>
#include <thread>

#include "application.hpp"
#include "../additionalSystem/printer.hpp"

class source
{
  public:
    source() = default;
    source(const source & src);
    source(source && src);
    source & operator=(const source & src);
    source & operator=(source && src);
    ~source();
    source(size_t id, size_t priority, size_t sleepTime, double lambda, printer * print);

    std::shared_ptr< application > returnApp();
    void stepWork(double stepTime);
    void autoWork();
    void stopAutoWork();

  private:
    static inline size_t actualAppId_;
    size_t id_, priority_, sleepTime_;
    double workTime_, lambda_, timeToNextApp_;
    std::thread thread_;
    std::atomic< bool > isRunning_{false};
    std::chrono::time_point< std::chrono::high_resolution_clock > initTime_;
    std::queue< std::shared_ptr< application > > apps_;
    printer * print_;
    void createNewApp();
    void autoWorkThread();
};

#endif
