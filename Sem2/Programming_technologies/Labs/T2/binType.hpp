#ifndef BINTYPE_HPP
#define BINTYPE_HPP

#include <iostream>

namespace rebdev
{
  struct BinTypeIO
  {
    unsigned long long & data;
  };

  std::istream & operator>>(std::istream & in, BinTypeIO && bin);
  std::ostream & operator<<(std::ostream & out, BinTypeIO && bin);
}

#endif
