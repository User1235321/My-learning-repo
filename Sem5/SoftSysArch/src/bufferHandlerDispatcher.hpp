#ifndef BUFFERHANDLERDISPATCHER_HPP
#define BUFFERHANDLERDISPATCHER_HPP

#include <vector>
#include <thread>
#include <atomic>
#include <mutex>

#include "buffer.hpp"
#include "handler.hpp"

class bufferHandlerDispatcher
{
  public:
    bufferHandlerDispatcher(std::vector< handler > & handlers, buffer & buff, std::ostream * out, size_t sleepTime);
    ~bufferHandlerDispatcher();

    void stepWork();
    void autoWork();
    void stopAutoWork();

  private:
    std::vector< handler > & handlers_;
    buffer & buff_;
    std::ostream * out_;
    size_t handlerNum_;

    std::thread thread_;
    std::atomic< bool > isRunning_{false};
    std::mutex mutex_;
    size_t sleepTime_;

    void dispatcherThreadFunc();
};

#endif
