#ifndef SOURCEBUFFERDISPATCHER_HPP
#define SOURCEBUFFERDISPATCHER_HPP

#include <vector>
#include <thread>
#include <atomic>
#include <mutex>

#include "source.hpp"
#include "buffer.hpp"

class sourceBufferDispatcher
{
  public:
    sourceBufferDispatcher(std::vector< source > & sources, std::vector< buffer > & buff, std::ostream * out, size_t sleepTime);
    ~sourceBufferDispatcher();

    sourceBufferDispatcher(const sourceBufferDispatcher & sbd);
    sourceBufferDispatcher(sourceBufferDispatcher && sbd) noexcept;
    sourceBufferDispatcher & operator=(const sourceBufferDispatcher & sbd);
    sourceBufferDispatcher & operator=(sourceBufferDispatcher && sbd) noexcept;

    void stepWork();
    void autoWork();
    void stopAutoWork();

    void replaceOut(std::ostream * out);
  private:
    std::vector< source > & sources_;
    std::vector< buffer > & buff_;
    std::ostream * out_;

    std::thread thread_;
    std::atomic< bool > isRunning_{false};
    std::mutex mutex_;
    size_t sleepTime_;

    size_t actualBuff_;

    void dispatcherThreadFunc();
};

#endif
