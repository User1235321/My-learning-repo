#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <cstddef>
#include <stdexcept>
#include <memory>

#include <list.hpp>

namespace rebdev
{
  template < class T >
  class Queue
  {
    public:
      Queue() = default;
      Queue(const Queue & queue) = default;
      Queue(Queue && queue) = default;
      ~Queue() = default;
      Queue & operator=(const Queue & queue) = default;
      Queue & operator=(Queue && rhQueue) = default;
      void push(T data)
      {
        dataBase_.push_back(data);
      }
      std::shared_ptr< T > drop()
      {
        if (dataBase_.size() == 0)
        {
          throw std::logic_error("Try to take and delete element from empty queue!");
        }
        std::shared_ptr< T > ptr(new T{dataBase_.front()});
        dataBase_.pop_front();
        return ptr;
      }
      size_t size() const noexcept
      {
        return dataBase_.size();
      }
      bool empty() const noexcept
      {
        return dataBase_.empty();
      }
    private:
      BiList< T > dataBase_;
  };
}

#endif
