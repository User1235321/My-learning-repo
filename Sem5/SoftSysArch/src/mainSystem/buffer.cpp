#include "buffer.hpp"

#include <limits>

buffer::buffer(size_t id, size_t limit, printer * print):
  id_(id),
  limit_(limit),
  numberOfOccupiedCells_(0),
  print_(print)
  {
    for (size_t i = 0; i< limit; ++i)
    {
      apps_.push_back(std::shared_ptr< application >(nullptr));
    }
  }

void buffer::push(std::shared_ptr< application > app)
{
  print_ -> printBufferIn(id_, app);
  if(numberOfOccupiedCells_ < limit_)
  {
    apps_[numberOfOccupiedCells_] = app;
    ++numberOfOccupiedCells_;
  }
  else
  {
    print_ -> printBufferDel(id_, apps_[0]);
    for (size_t i = 0; i < (numberOfOccupiedCells_ - 1); ++i)
    {
      apps_[i] = apps_[i + 1];
    }
    apps_[numberOfOccupiedCells_ - 1] = app;
  }
}

std::shared_ptr< application > buffer::pop()
{
  int maxPriority = std::numeric_limits< int >::max(), minId = std::numeric_limits< int >::max(), index = 0;
  for (size_t i = 0; i < numberOfOccupiedCells_; ++i)
  {
    if ((*(apps_[i])).priority_ < maxPriority)
    {
      maxPriority = (*(apps_[i])).priority_;
      minId = (*(apps_[i])).id_;
      index = i;
    }
    else if (((*(apps_[i])).priority_ == maxPriority) && ((*(apps_[i])).id_ < minId))
    {
      minId = (*(apps_[i])).id_;
      index = i;
    }
  }
  std::shared_ptr< application > app = apps_[index];
  if (app.get() != nullptr)
  {
    print_ -> printBufferOut(id_);
    numberOfOccupiedCells_ -= 1;
    apps_[index] = nullptr;
    for (size_t i = index; i < numberOfOccupiedCells_; ++i)
    {
      apps_[i] = apps_[i + 1];
    }
  }
  return app;
}
