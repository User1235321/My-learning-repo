#ifndef BIDIRECTIONALITERATOR_HPP
#define BIDIRECTIONALITERATOR_HPP

#include <memory>
#include <iterator>

#include "biListNode.hpp"

namespace rebdev
{
  template < class T >
  class BiList;

  template < class T >
  class BidirectionalIterator: public std::iterator< std::bidirectional_iterator_tag, T >
  {
    friend BiList< T >;
    using iterator = BidirectionalIterator< T >;
    using node = BiListNode< T >;

    public:
      BidirectionalIterator() = default;
      BidirectionalIterator(const iterator & it) = default;
      BidirectionalIterator(iterator && it) = default;

      ~BidirectionalIterator() noexcept = default;

      iterator & operator=(const iterator & it) noexcept = default;
      iterator & operator=(iterator && it) noexcept = default;

      bool operator==(const iterator & it) const noexcept
      {
        return (node_ == it.node_);
      }
      bool operator!=(const iterator & it) const noexcept
      {
        return !(*this == it);
      }

      T & operator*() const
      {
        return node_->data;
      }
      T * operator->() const
      {
        return std::addressof(node_->data);
      }

      iterator& operator++()
      {
        node_ = node_->next;
        return *this;
      }
      iterator operator++(int)
      {
        iterator oldIter(node_);
        ++node_;
        return oldIter;
      }
      iterator& operator--()
      {
        node_ = node_->last;
        return *this;
      }
      iterator operator--(int)
      {
        iterator oldIter(node_);
        --node_;
        return oldIter;
      }

    private:
      node * node_;
      explicit BidirectionalIterator(node * originalNode) noexcept:
        node_(originalNode)
      {}
  };
}
#endif
