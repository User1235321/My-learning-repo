#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <iostream>
#include <vector>

namespace rebdev
{
  struct Point
  {
    int x, y;
  };
  struct Polygon
  {
    std::vector< Point > points;
  };
  std::istream & operator>>(std::istream & in, Point & point);
  std::istream & operator>>(std::istream & in, Polygon & poly);
  double polygonArea(const Polygon & poly);
  size_t polygonVert(const Polygon & poly) noexcept;
}

#endif
