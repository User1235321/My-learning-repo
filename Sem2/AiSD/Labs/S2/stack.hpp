#ifndef STACK_HPP
#define STACH_HPP

#include <cstddef>
#include <stdexcept>
#include <memory>

#include <list.hpp>

namespace rebdev
{
  template < class T >
  class Stack
  {
    public:
      Stack() = default;
      Stack(const Stack & stack) = default;
      Stack(Stack && stack) = default;
      ~Stack() = default;
      Stack & operator=(const Stack & stack) = default;
      Stack & operator=(Stack && rhStack) = default;
      void push(T data)
      {
        dataBase_.push_back(data);
      }
      std::shared_ptr< T > drop()
      {
        if (dataBase_.size() == 0)
        {
          throw std::logic_error("Try to take and delete element from empty stack!");
        }
        std::shared_ptr< T > ptr (new T{dataBase_.back()});
        dataBase_.pop_back();
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
