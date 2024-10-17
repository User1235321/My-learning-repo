#ifndef CONSTBIDIRECTIONALITERATOR_HPP
#define CONSTBIDIRECTIONALITERATOR_HPP

#include <memory>
#include <iterator>

#include "biListNode.hpp"

namespace rebdev
{
  template < class T >
  class BiList;

  template < class T >
  class ConstBidirectionalIterator: public std::iterator< std::bidirectional_iterator_tag, T >
  {
    friend BiList< T >;
    using const_iterator = ConstBidirectionalIterator< T >;
    using node = BiListNode< T >;

    public:
      ConstBidirectionalIterator() = default;
      ConstBidirectionalIterator(const const_iterator & c_it) = default;
      ConstBidirectionalIterator(const_iterator && c_it) = default;

      ~ConstBidirectionalIterator() noexcept = default;

      const_iterator & operator=(const const_iterator & c_it) noexcept = default;
      const_iterator & operator=(const_iterator && c_it) noexcept = default;

      bool operator==(const const_iterator & c_it) const noexcept
      {
        return (node_ == c_it.node_);
      }
      bool operator!=(const const_iterator & c_it) const noexcept
      {
        return !(*this == c_it);
      }

      const T & operator*() const
      {
        return node_->data;
      }
      const T * operator->() const
      {
        return std::addressof(node_->data);
      }

      const_iterator & operator++()
      {
        node_ = node_->next;
        return *this;
      }
      const_iterator operator++(int)
      {
        const_iterator oldIter(node_);
        ++node_;
        return oldIter;
      }
      const_iterator & operator--()
      {
        node_ = node_->last;
        return *this;
      }
      const_iterator operator--(int)
      {
        const_iterator oldIter(node_);
        --node_;
        return oldIter;
      }

    private:
      const node * node_;

      explicit ConstBidirectionalIterator(const node * originalNode) noexcept:
        node_(originalNode)
      {}
  };
}
#endif
