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
    bufferHandlerDispatcher(std::vector< handler > & handlers, std::vector< buffer > & buff, std::ostream * out, size_t sleepTime, std::mutex * outMutex);
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
    size_t handlerNum_, sleepTime_, actualBuff_;
    std::vector< handler > & handlers_;
    std::vector< buffer > & buff_;
    std::ostream * out_;
    std::thread thread_;
    std::atomic< bool > isRunning_{false};
    std::mutex * outMutex_;
    void dispatcherThreadFunc();
};

#endif
