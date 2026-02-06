#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include "polygon.hpp"

namespace rebdev
{
  using polyVec = std::vector< Polygon >;
  void areaBase(std::istream & in, std::ostream & out, const polyVec & vec);
  void maxBase(std::istream & in, std::ostream & out, const polyVec & vec);
  void minBase(std::istream & in, std::ostream & out, const polyVec & vec);
  void countBase(std::istream & in, std::ostream & out, const polyVec & vec);
  void rects(std::istream &, std::ostream & out, const polyVec & vec);
  void inframe(std::istream & in, std::ostream & out, const polyVec & vec);
}

#endif
