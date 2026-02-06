#ifndef SOURCE_HPP
#define SOURCE_HPP

#include <atomic>
#include <chrono>
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
    source(size_t id, size_t priority, size_t sleepTime, long double lambda, printer * print);

    application returnApp();
    void stepWork(long double stepTime);
    void autoWork();
    void stopAutoWork();

  private:
    size_t id_, priority_, sleepTime_;
    long double workTime_, lambda_, timeToNextApp_;
    std::thread thread_;
    static inline std::atomic< size_t > actualAppId_{0};
    std::atomic< bool > isRunning_{false};
    std::chrono::time_point< std::chrono::high_resolution_clock > initTime_;
    std::queue< application > apps_;
    printer * print_;
    void createNewApp();
    void autoWorkThread();
};

#endif
