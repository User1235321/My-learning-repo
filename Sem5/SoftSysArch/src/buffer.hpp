#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <iostream>
#include <memory>
#include <vector>
#include <mutex>

#include "application.hpp"

class buffer
{
  public:
    buffer(std::ostream * out, size_t limit, size_t sleepTime, size_t priorNumi, std::mutex * outMutex);
    buffer(const buffer &) = default;
    buffer(buffer &&) = default;
    buffer & operator=(const buffer &) = default;
    buffer & operator=(buffer &&) = default;

    void push(std::shared_ptr< application > app);
    std::shared_ptr< application > pop();
    size_t getDelNum() const noexcept;
    size_t getFullness() const noexcept;
    size_t getLimit() const noexcept;
    void replaceOut(std::ostream * out);
    std::vector< size_t > returnPriorDelNum() const;
    std::vector< double > returnTime() const;

  private:
    size_t limit_, numberOfOccupiedCells_, sleepTime_, numOfDel_;
    std::vector< std::shared_ptr< application > > apps_;
    std::ostream * out_;
    std::mutex * outMutex_;
    std::vector< size_t > priorDelNum_;
    std::vector< double > times_;
};

#endif
