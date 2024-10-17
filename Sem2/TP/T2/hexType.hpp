#ifndef HEXTYPE_HPP
#define HEXTYPE_HPP

#include <iostream>

namespace rebdev
{
  struct HexTypeIO
  {
    unsigned long long & data;
  };

  std::istream & operator>>(std::istream & in, HexTypeIO && hex);
  std::ostream & operator<<(std::ostream & out, HexTypeIO && hex);
}

#endif
