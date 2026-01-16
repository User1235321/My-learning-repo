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
    bufferHandlerDispatcher(std::vector< handler > & handlers, std::vector< buffer > & buff, std::ostream * out, size_t sleepTime);
    ~bufferHandlerDispatcher();

    bufferHandlerDispatcher(const bufferHandlerDispatcher & bhd);
    bufferHandlerDispatcher(bufferHandlerDispatcher && bhd) noexcept;
    bufferHandlerDispatcher & operator=(const bufferHandlerDispatcher & bhd);
    bufferHandlerDispatcher & operator=(bufferHandlerDispatcher && bhd) noexcept;

    void stepWork();
    void autoWork();
    void stopAutoWork();

    void replaceOut(std::ostream * out);

  private:
    std::vector< handler > & handlers_;
    std::vector< buffer > & buff_;
    std::ostream * out_;
    size_t handlerNum_;

    std::thread thread_;
    std::atomic< bool > isRunning_{false};
    std::mutex mutex_;
    size_t sleepTime_;

    size_t actualBuff_;

    void dispatcherThreadFunc();
};

#endif
