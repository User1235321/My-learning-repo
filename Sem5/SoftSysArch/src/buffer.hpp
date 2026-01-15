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

    void push(std::shared_ptr< application > app);
    std::shared_ptr< application > pop();

  private:
    std::vector< std::shared_ptr< application > > apps_;
    std::ostream * out_;
    size_t limit_;
    size_t numberOfOccupiedCells_;
    size_t sleepTime_;
};

#endif
