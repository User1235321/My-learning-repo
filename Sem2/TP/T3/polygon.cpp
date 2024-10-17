#include "polygon.hpp"

#include <algorithm>
#include <limits>
#include <iterator>
#include <stdexcept>
#include <sstream>
#include <string>
#include <numeric>

#include <delimeter.hpp>

class SquareGenerator
{
  public:
    explicit SquareGenerator(const rebdev::Polygon & p) noexcept:
      safedPoly(p),
      pointNow(1)
    {}
    double operator()() noexcept
    {
      double area = safedPoly.points[0].x * (safedPoly.points[pointNow].y - safedPoly.points[(pointNow + 1)].y);
      area += safedPoly.points[pointNow].x * (safedPoly.points[(pointNow + 1)].y - safedPoly.points[0].y);
      area += safedPoly.points[(pointNow + 1)].x * (safedPoly.points[0].y - safedPoly.points[pointNow].y);
      area /= 2;
      pointNow += 1;
      return std::abs(area);
    }
  private:
    const rebdev::Polygon & safedPoly;
    size_t pointNow;
};
double rebdev::polygonArea(const Polygon & poly)
{
  if (poly.points.empty())
  {
    return 0;
  }
  size_t triangleNum = (poly.points.size() - 2);
  std::vector< double > squares(triangleNum);
  std::generate_n(squares.begin(), triangleNum, SquareGenerator(poly));
  return std::accumulate(squares.begin(), squares.end(), 0);
}
std::istream & rebdev::operator>>(std::istream & in, Point & point)
{
  in >> Delimeter{'('} >> point.x >> Delimeter{';'} >> point.y >> Delimeter{')'};
  return in;
}
std::istream & rebdev::operator>>(std::istream & in, Polygon & poly)
{
  poly.points.clear();
  std::string str;
  std::getline(in, str);
  if (str.empty())
  {
    return in;
  }
  std::stringstream inStr(str);
  size_t pointNum = 0;
  inStr >> pointNum;
  if (inStr.eof() || (pointNum < 3))
  {
    return in;
  }
  using inputItT = std::istream_iterator< Point >;
  Polygon newPoly;
  newPoly.points.insert(newPoly.points.begin(), inputItT{ inStr }, inputItT{});
  if ((inStr.fail() && !inStr.eof()) || (pointNum < 3) || (newPoly.points.size() != pointNum))
  {
    newPoly.points.clear();
  }
  std::swap(newPoly, poly);
  return in;
}
size_t rebdev::polygonVert(const Polygon & poly) noexcept
{
  return poly.points.size();
}
