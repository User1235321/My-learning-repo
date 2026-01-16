#include "buffer.hpp"

#include <limits>

buffer::buffer(std::ostream * out, size_t limit, size_t sleepTime):
  out_(out),
  limit_(limit),
  sleepTime_(sleepTime),
  numberOfOccupiedCells_(0),
  numOfDel_(0)
  {
    for(size_t i = 0; i < limit; ++i)
    {
      apps_.emplace_back(nullptr);
    }
  }

void buffer::push(std::shared_ptr< application > app)
{
  (*app).actualStageNum_ = 3;
  (*out_) << "\033[1;33mЗаявка с id " << (*app).id_ << " полжена в буфер\033[0m\n";
  if(numberOfOccupiedCells_ < limit_)
  {
    apps_[numberOfOccupiedCells_] = app;
    ++numberOfOccupiedCells_;
  }
  else
  {
    (*out_) << "\033[1;34mЗаявка с id " << (*apps_[0]).id_ << " удалена из буфера из-за переполнения\033[0m\n";
    for (size_t i = 0; i < (numberOfOccupiedCells_ - 1); ++i)
    {
      apps_[i] = apps_[i + 1];
    }
    apps_[numberOfOccupiedCells_ - 1] = app;
    ++numOfDel_;
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
    numberOfOccupiedCells_ -= 1;
    apps_[index] = nullptr;
    for (size_t i = index; i < numberOfOccupiedCells_; ++i)
    {
      apps_[i] = apps_[i + 1];
    }
  }
  return app;
}

size_t buffer::getDelNum() const noexcept
{
  return numOfDel_;
}

size_t buffer::getFullness() const noexcept
{
  return numberOfOccupiedCells_;
}

size_t buffer::getLimit() const noexcept
{
  return limit_;
}

void buffer::replaceOut(std::ostream * out)
{
  out_ = out;
}
