#ifndef SOURCEBUFFERDISPATCHER_HPP
#define SOURCEBUFFERDISPATCHER_HPP

#include <atomic>
#include <thread>
#include <vector>

#include "buffer.hpp"
#include "source.hpp"

class sourceBufferDispatcher
{
  public:
    sourceBufferDispatcher() = default;
    sourceBufferDispatcher(const sourceBufferDispatcher & sbd);
    sourceBufferDispatcher(sourceBufferDispatcher && sbd);
    sourceBufferDispatcher & operator=(const sourceBufferDispatcher & sbd);
    sourceBufferDispatcher & operator=(sourceBufferDispatcher && sbd);
    ~sourceBufferDispatcher();
    sourceBufferDispatcher(size_t sleepTime, std::vector< source > * sources, std::vector< buffer > * buff);

    void stepWork();
    void autoWork();
    void stopAutoWork();

  private:
    size_t actualBuff_, sleepTime_;
    std::vector< source > * sources_;
    std::vector< buffer > * buff_;
    std::thread thread_;
    std::atomic< bool > isRunning_{false};
    void dispatcherThreadFunc();
};

#endif
