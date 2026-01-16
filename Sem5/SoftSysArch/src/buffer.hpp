#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <iostream>
#include <memory>
#include <vector>

#include "application.hpp"

class buffer
{
  public:
    buffer(std::ostream * out, size_t limit, size_t sleepTime);

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

  private:
    std::vector< std::shared_ptr< application > > apps_;
    std::ostream * out_;
    size_t limit_;
    size_t numberOfOccupiedCells_;
    size_t sleepTime_;
    size_t numOfDel_;
};

#endif
