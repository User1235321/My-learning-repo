#ifndef LIST_HPP
#define LIST_HPP

#include "BidirectionalIterator.hpp"
#include "constBidirectionalIterator.hpp"

namespace rebdev
{
  template < class T >
  class BiList
  {
    using node = BiListNode< T >;

    public:
      using iterator = BidirectionalIterator< T >;
      using const_iterator = ConstBidirectionalIterator< T >;

      BiList():
        headNode_(new node{T{}, nullptr, nullptr}),
        tailNode_(headNode_),
        size_(0)
      {}
      BiList(size_t n, const T & val):
        headNode_(new node{T{}, nullptr, nullptr}),
        tailNode_(headNode_),
        size_(0)
      {
        try
        {
          assign(n, val);
        }
        catch (...)
        {
          delete headNode_;
          throw;
        }
      }
      BiList(size_t n):
        BiList(n, T{})
      {}
      BiList(const BiList & list):
        headNode_(new node{T{}, nullptr, nullptr}),
        tailNode_(headNode_),
        size_(0)
      {
        const_iterator it = list.cbegin();

        while (it != list.cend())
        {
          try
          {
            push_back(*it);
          }
          catch (...)
          {
            clear();
            throw;
          }
          ++it;
        }
      }
      BiList(BiList && list):
        headNode_(list.headNode_),
        tailNode_(list.tailNode_),
        size_(list.size_)
      {
        list.headNode_ = nullptr;
        list.tailNode_ = nullptr;
      }
      ~BiList() noexcept
      {
        clear();
        delete headNode_;
      }

      BiList & operator=(const BiList & list)
      {
        if (*this != list)
        {
          BiList< T > lst(list);
          swap(lst);
        }
        return *this;
      }
      BiList & operator=(BiList && list)
      {
        if (*this != std::addressof(list))
        {
          BiList< T > lst(std::move(list));
          swap(lst);
        }
        return *this;
      }

      iterator begin()
      {
        return iterator(headNode_);
      }
      const_iterator cbegin() const
      {
        return const_iterator(headNode_);
      }
      iterator end()
      {
        return iterator(tailNode_);
      }
      const_iterator cend() const
      {
        return const_iterator(tailNode_);
      }

      bool empty() const noexcept
      {
        return (headNode_ == tailNode_);
      }
      size_t size() const noexcept
      {
        return size_;
      }

      T & front()
      {
        return headNode_->data;
      }
      T & back()
      {
        return tailNode_->last->data;
      }
      const T & front() const
      {
        const T & ref = headNode_->data;
        return ref;
      }
      const T & back() const
      {
        const T & ref = tailNode_->last->data;
        return ref;
      }

      void assign(size_t n, const T & val)
      {
        BiList< T > newList;
        for (size_t i = 0; i < n; ++i)
        {
          newList.push_back(val);
        }
        swap(newList);
      }
      void push_front(const T & val)
      {
        push_front(new node{val, nullptr, headNode_});
      }
      void push_front(T && val)
      {
        push_front(new node{std::move(val), nullptr, headNode_});
      }
      void pop_front()
      {
        node * headCopy = headNode_->next;
        delete headNode_;
        headNode_ = headCopy;
        headNode_->last = nullptr;
        --size_;
      }
      void push_back(const T & val)
      {
        push_back(new node{val, tailNode_->last, tailNode_});
      }
      void push_back(T && val)
      {
        push_back(new node{std::move(val), tailNode_->last, tailNode_});
      }
      void pop_back()
      {
        node * tailCopy = tailNode_->last->last;
        delete tailNode_->last;
        tailNode_->last = tailCopy;
        if (tailCopy != nullptr)
        {
          tailCopy->next = tailNode_;
        }
        else
        {
          headNode_ = tailNode_;
        }
        --size_;
      }
      void swap(BiList & x) noexcept
      {
        std::swap(headNode_, x.headNode_);
        std::swap(tailNode_, x.tailNode_);
        std::swap(size_, x.size_);
      }
      void clear() noexcept
      {
        while (headNode_ != tailNode_)
        {
          node * nextNode = headNode_->next;
          delete headNode_;
          headNode_ = nextNode;
        }
      }

      void remove(const T & val)
      {
        remove_if(
          [&](const T & current)
          {
            return (current == val);
          });
      }
      template < class Predicate >
      void remove_if(Predicate pred)
      {
        iterator it = begin();
        while (it != end())
        {
          if (pred(*it))
          {
            node * lastNode = it.node_->last;
            node * nextNode = it.node_->next;
            delete it.node_;

            if (lastNode == nullptr)
            {
              headNode_ = nextNode;
            }
            else
            {
              lastNode->next = nextNode;
            }
            nextNode->last = lastNode;
            it = iterator(nextNode);
            --size_;
          }
          else
          {
            ++it;
          }
        }
      }
    private:
      node * headNode_;
      node * tailNode_;
      size_t size_;

      void push_back(node * n) noexcept
      {
        if ((tailNode_->last) != nullptr)
        {
          tailNode_->last->next = n;
        }
        else
        {
          headNode_ = n;
        }
        tailNode_->last = n;
        ++size_;
      }
      void push_front(node * n) noexcept
      {
        headNode_->last = n;
        headNode_ = n;
        ++size_;
      }
  };
}

#endif
