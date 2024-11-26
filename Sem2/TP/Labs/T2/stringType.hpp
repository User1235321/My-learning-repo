#ifndef STRINGTYPE_HPP
#define STRINGTYPE_HPP

#include <iostream>
#include <string>

namespace rebdev
{
  struct StringTypeIO
  {
    std::string & data;
  };

  std::istream & operator>>(std::istream & in, StringTypeIO && str);
  std::ostream & operator<<(std::ostream & out, StringTypeIO && str);
}

#endif

