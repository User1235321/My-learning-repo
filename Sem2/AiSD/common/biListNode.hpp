#ifndef BILISTNODE_HPP
#define BILISTNODE_HPP

namespace rebdev
{
  template < class T >
  struct BiListNode
  {
    T data;
    BiListNode * last;
    BiListNode * next;
  };
}

#endif
