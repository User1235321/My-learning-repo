#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <mutex>
#include <vector>

#include "application.hpp"
#include "../additionalSystem/printer.hpp"

class buffer
{
  public:
    buffer() = default;
    buffer(const buffer & buff);
    buffer(buffer && buff);
    buffer & operator=(const buffer & buff);
    buffer & operator=(buffer && buff);
    ~buffer() = default;
    buffer(size_t id, size_t limit, printer * print);

    void push(application app);
    application pop();

  private:
    size_t id_, limit_, numberOfOccupiedCells_;
    std::vector< application > apps_;
    mutable std::mutex mutex_;
    printer * print_;
};

#endif
