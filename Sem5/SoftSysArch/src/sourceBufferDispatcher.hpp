#ifndef SOURCEBUFFERDISPATCHER_HPP
#define SOURCEBUFFERDISPATCHER_HPP

#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <memory>

#include "source.hpp"
#include "buffer.hpp"

class sourceBufferDispatcher
{
  public:
    sourceBufferDispatcher(std::vector< source > & sources, buffer & buff, std::ostream * out, size_t sleepTime);
    ~sourceBufferDispatcher();

    void stepWork();
    void autoWork();
    void stopAutoWork();

  private:
    std::vector< source > & sources_;
    buffer & buff_;
    std::ostream * out_;

    std::thread dispatcherThread_;
    std::atomic< bool > isRunning_{false};
    std::mutex workMutex_;
    size_t sleepTime_;

    void dispatcherThreadFunc();
};

#endif
