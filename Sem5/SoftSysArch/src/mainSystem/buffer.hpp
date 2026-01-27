#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <memory>
#include <vector>

#include "application.hpp"
#include "../additionalSystem/printer.hpp"

class buffer
{
  public:
    buffer() = default;
    buffer(const buffer &) = default;
    buffer(buffer &&) = default;
    buffer & operator=(const buffer &) = default;
    buffer & operator=(buffer &&) = default;
    ~buffer() = default;
    buffer(size_t id, size_t limit, printer * print);

    void push(std::shared_ptr< application > app);
    std::shared_ptr< application > pop();

  private:
    size_t id_, limit_, numberOfOccupiedCells_;
    std::vector< std::shared_ptr< application > > apps_;
    printer * print_;
};

#endif
