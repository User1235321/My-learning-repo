#include "buffer.hpp"

#include <limits>

buffer::buffer(std::ostream * out, size_t limit, size_t sleepTime):
  out_(out),
  limit_(limit),
  sleepTime_(sleepTime),
  numberOfOccupiedCells_(0)
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
  if (numberOfOccupiedCells_ < limit_)
  {
    int i = 0;
    while ((apps_[i]).get() != nullptr)
    {
      ++i;
    }
    apps_[i] = app;
    ++numberOfOccupiedCells_;
  }
  else
  {
    int minId = std::numeric_limits< int >::max(), index = 0;
    for (size_t i = 0; i < limit_; ++i)
    {
      if ((*(apps_[i])).id_ < minId)
      {
        minId = (*(apps_[i])).id_;
        index = i;
      }
    }
    (*out_) << "\033[1;34mЗаявка с id " << minId << " удалена из буфера из-за переполнения\033[0m\n";
    apps_[index] = app;
  }
}
std::shared_ptr< application > buffer::pop()
{
  int maxPriority = std::numeric_limits< int >::max(), minId = std::numeric_limits< int >::max(), index = 0;
  for (size_t i = 0; i < limit_; ++i)
  {
    if ((apps_[i]).get() != nullptr)
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
  }
  std::shared_ptr< application > app = apps_[index];
  apps_[index] = nullptr;
  numberOfOccupiedCells_ -= (app.get() != nullptr);
  return app;
}
