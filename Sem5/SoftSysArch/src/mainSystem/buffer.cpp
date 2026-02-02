#include "buffer.hpp"

#include <chrono>
#include <limits>

buffer::buffer(const buffer & buff):
  id_(buff.id_),
  limit_(buff.limit_),
  numberOfOccupiedCells_(buff.numberOfOccupiedCells_),
  apps_(buff.apps_),
  print_(buff.print_){}

buffer::buffer(buffer && buff):
  id_(buff.id_),
  limit_(buff.limit_),
  numberOfOccupiedCells_(buff.numberOfOccupiedCells_),
  apps_(std::move(buff.apps_)),
  print_(buff.print_){}

buffer & buffer::operator=(const buffer & buff)
{
  if (&buff != this)
  {
    id_ = buff.id_;
    limit_ = buff.limit_;
    numberOfOccupiedCells_ = buff.numberOfOccupiedCells_;
    apps_ = buff.apps_;
    print_ = buff.print_;
  }
  return *this;
}

buffer & buffer::operator=(buffer && buff)
{
  if (&buff != this)
  {
    id_ = buff.id_;
    limit_ = buff.limit_;
    numberOfOccupiedCells_ = buff.numberOfOccupiedCells_;
    apps_ = std::move(buff.apps_);
    print_ = buff.print_;
  }
  return *this;
}

buffer::buffer(size_t id, size_t limit, printer * print):
  id_(id),
  limit_(limit),
  numberOfOccupiedCells_(0),
  print_(print)
  {
    for (size_t i = 0; i < limit; ++i)
    {
      apps_.push_back(application{0, 0, 0, std::chrono::time_point< std::chrono::high_resolution_clock >()});
    }
  }

void buffer::push(application app)
{
  bool del = false;
  application delApp = application{0, 0, 0, std::chrono::time_point< std::chrono::high_resolution_clock >()};
  {
    std::lock_guard< std::mutex > lock(mutex_);
    if(numberOfOccupiedCells_ < limit_)
    {
      apps_[numberOfOccupiedCells_] = app;
      ++numberOfOccupiedCells_;
    }
    else
    {
      del = true;
      delApp = apps_[0];
      for (size_t i = 0; i < (numberOfOccupiedCells_ - 1); ++i)
      {
        apps_[i] = apps_[i + 1];
      }
      apps_[numberOfOccupiedCells_ - 1] = app;
    }
  }
  print_ -> printBufferIn(id_, app);
  if (del)
  {
    print_ -> printBufferDel(id_, delApp);
  }
}

application buffer::pop()
{
  bool good = false;
  application app{0, 0, 0, std::chrono::time_point< std::chrono::high_resolution_clock >()};
  {
    std::lock_guard< std::mutex > lock(mutex_);
    int maxPriority = std::numeric_limits< int >::max(), minId = std::numeric_limits< int >::max(), index = 0;
    for (size_t i = 0; i < numberOfOccupiedCells_; ++i)
    {
      if ((apps_[i]).priority_ < maxPriority)
      {
        maxPriority = (apps_[i]).priority_;
        minId = (apps_[i]).id_;
        index = i;
      }
      else if (((apps_[i]).priority_ == maxPriority) && ((apps_[i]).id_ < minId))
      {
        minId = (apps_[i]).id_;
        index = i;
      }
    }
    app = apps_[index];
    if (app.id_ != 0)
    {
      good = true;
      for (size_t i = index; i < numberOfOccupiedCells_; ++i)
      {
        apps_[i] = apps_[i + 1];
      }
      numberOfOccupiedCells_ -= 1;
      apps_[numberOfOccupiedCells_] = application{0, 0, 0, std::chrono::time_point< std::chrono::high_resolution_clock >()};
    }
  }
  if (good)
  {
    print_ -> printBufferOut(id_);
  }
  return app;
}
