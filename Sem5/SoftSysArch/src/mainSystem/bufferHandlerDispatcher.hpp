#ifndef BUFFERHANDLERDISPATCHER_HPP
#define BUFFERHANDLERDISPATCHER_HPP

#include <atomic>
#include <thread>
#include <vector>

#include "buffer.hpp"
#include "handler.hpp"

class bufferHandlerDispatcher
{
  public:
    bufferHandlerDispatcher() = default;
    bufferHandlerDispatcher(const bufferHandlerDispatcher & bhd);
    bufferHandlerDispatcher(bufferHandlerDispatcher && bhd);
    bufferHandlerDispatcher & operator=(const bufferHandlerDispatcher & bhd);
    bufferHandlerDispatcher & operator=(bufferHandlerDispatcher && bhd);
    ~bufferHandlerDispatcher();
    bufferHandlerDispatcher(size_t sleepTime, std::vector< handler > * handlers, std::vector< buffer > * buff);

    void stepWork();
    void autoWork();
    void stopAutoWork();

  private:
    size_t actualBuff_, handlerNum_, sleepTime_;
    std::vector< handler > * handlers_;
    std::vector< buffer > * buff_;
    std::thread thread_;
    std::atomic< bool > isRunning_{false};
    void dispatcherThreadFunc();
};

#endif
