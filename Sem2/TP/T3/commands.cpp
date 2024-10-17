#include "commands.hpp"

#include <string>
#include <algorithm>
#include <functional>
#include <numeric>
#include <cmath>
#include <utility>

bool areaNum(const rebdev::Polygon & poly, size_t num) noexcept
{
  return (poly.points.size() == num);
}
bool areaEven(const rebdev::Polygon & poly) noexcept
{
  return (((poly.points.size() % 2) == 0) && (!poly.points.empty()));
}
bool areaOdd(const rebdev::Polygon & poly) noexcept
{
  return ((poly.points.size() % 2) != 0);
}
void rebdev::areaBase(std::istream & in, std::ostream & out, const polyVec & vec)
{
  std::string str;
  in >> str;
  polyVec vecCopy(vec.size());
  polyVec::iterator it;
  try
  {
    size_t num = std::stoull(str);
    if (num < 3)
    {
      throw std::logic_error("Uncorect area num parametr!");
    }
    auto areaN = std::bind(areaNum, std::placeholders::_1, num);
    it = std::copy_if(vec.begin(), vec.end(), vecCopy.begin(), areaN);
  }
  catch (const std::exception & e)
  {
    if (str == "EVEN")
    {
      it = std::copy_if(vec.begin(), vec.end(), vecCopy.begin(), areaEven);
    }
    else if (str == "ODD")
    {
      it = std::copy_if(vec.begin(), vec.end(), vecCopy.begin(), areaOdd);
    }
    else if (str == "MEAN")
    {
      if (vecCopy.empty())
      {
        throw std::logic_error("Can't calcute mean area without figures!");
      }
      vecCopy = vec;
      it = vecCopy.end();
    }
    else
    {
      throw std::out_of_range("Unknown command!");
    }
  }
  std::vector< double > areaVec(vec.size());
  std::transform(vecCopy.begin(), it, areaVec.begin(), polygonArea);
  double area = std::accumulate(areaVec.begin(), areaVec.end(), 0);
  if (str == "MEAN")
  {
    area /= vec.size();
  }
  out << area << '\n';
}
using areaFunc = std::function< double(const std::vector< double > &) >;
using vertFunc = std::function< size_t(const std::vector< size_t > &) >;
void maxMinBase(std::istream & in, std::ostream & out, const rebdev::polyVec & vec, areaFunc ar, vertFunc vert)
{
  if (vec.empty())
  {
    throw std::logic_error("Can't calculate min or max without figures!");
  }
  std::string str;
  in >> str;
  if (str == "AREA")
  {
    std::vector< double > areaVec(vec.size());
    std::transform(vec.begin(), vec.end(), areaVec.begin(), rebdev::polygonArea);
    double area = ar(areaVec);
    out << area << '\n';
  }
  else if (str == "VERTEXES")
  {
    std::vector< size_t > vertexesVec(vec.size());
    std::transform(vec.begin(), vec.end(), vertexesVec.begin(), rebdev::polygonVert);
    size_t vertex = vert(vertexesVec);
    out << vertex << '\n';
  }
  else
  {
    throw std::out_of_range("Unknown command!");
  }
}
double maxArea(const std::vector< double > & areaVec)
{
  return (*std::max_element(areaVec.begin(), areaVec.end()));
}
double maxVertexes(const std::vector< size_t > & vertexesVec)
{
  return (*std::max_element(vertexesVec.begin(), vertexesVec.end()));
}
void rebdev::maxBase(std::istream & in, std::ostream & out, const polyVec & vec)
{
  maxMinBase(in, out, vec, maxArea, maxVertexes);
}
double minArea(const std::vector< double > & areaVec)
{
  return (*std::min_element(areaVec.begin(), areaVec.end()));
}
double minVertexes(const std::vector< size_t > & vertexesVec)
{
  return (*std::min_element(vertexesVec.begin(), vertexesVec.end()));
}
void rebdev::minBase(std::istream & in, std::ostream & out, const polyVec & vec)
{
  maxMinBase(in, out, vec, minArea, minVertexes);
}
bool countNum(size_t num, size_t mainNum) noexcept
{
  return (num == mainNum);
}
bool countEven(size_t num) noexcept
{
  return ((num % 2) == 0);
}
bool countOdd(size_t num) noexcept
{
  return ((num % 2) != 0);
}
void rebdev::countBase(std::istream & in, std::ostream & out, const polyVec & vec)
{
  std::string str;
  in >> str;
  std::vector< size_t > vertexesVec(vec.size());
  std::transform(vec.begin(), vec.end(), vertexesVec.begin(), polygonVert);
  size_t figureNum = 0;
  try
  {
    size_t num = std::stoull(str);
    if (num < 3)
    {
      throw std::logic_error("Uncorect count num parametr!");
    }
    auto countN = std::bind(countNum, std::placeholders::_1, num);
    figureNum = std::count_if(vertexesVec.begin(), vertexesVec.end(), countN);
  }
  catch (const std::exception & e)
  {
    if (str == "EVEN")
    {
      figureNum = std::count_if(vertexesVec.begin(), vertexesVec.end(), countEven);
    }
    else if (str == "ODD")
    {
      figureNum = std::count_if(vertexesVec.begin(), vertexesVec.end(), countOdd);
    }
    else
    {
      throw std::out_of_range("Unknown command!");
    }
  }
  out << figureNum << '\n';
}
double distance(const rebdev::Point & f, const rebdev::Point & s)
{
  return std::sqrt(std::pow((f.x - s.x), 2) + std::pow((f.y - s.y), 2));
}
bool isRect(const rebdev::Polygon & poly)
{
  if (poly.points.size() != 4)
  {
    return false;
  }
  return (distance(poly.points[0], poly.points[2]) == distance(poly.points[1], poly.points[3]));
}
void rebdev::rects(std::istream &, std::ostream & out, const polyVec & vec)
{
  out << std::count_if(vec.begin(), vec.end(), isRect) << '\n';
}
int getX(const rebdev::Point & p) noexcept
{
  return p.x;
}
int getY(const rebdev::Point & p) noexcept
{
  return p.y;
}
using getXOrY = std::function< int(const rebdev::Point &) >;
bool minXY(const rebdev::Point & f, const rebdev::Point & s, getXOrY xy) noexcept
{
  return xy(f) < xy(s);
}
using namespace std::placeholders;
bool minXYVec(const rebdev::Polygon & f, const rebdev::Polygon & s, getXOrY xy)
{
  auto min = std::bind(minXY, _1, _2, xy);
  auto fX = (*std::min_element(f.points.begin(), f.points.end(), min));
  auto sX = (*std::min_element(s.points.begin(), s.points.end(), min));
  return (xy(fX) < xy(sX));
}
bool maxXYVec(const rebdev::Polygon & f, const rebdev::Polygon & s, getXOrY xy)
{
  auto max = std::bind(minXY, _1, _2, xy);
  auto fX = (*std::max_element(f.points.begin(), f.points.end(), max));
  auto sX = (*std::max_element(s.points.begin(), s.points.end(), max));
  return (xy(fX) < xy(sX));
}
bool pointOutFrame(const rebdev::Point & p, const std::pair< rebdev::Point, rebdev::Point > & pair)
{
  return ((p.x < pair.first.x) || (p.y < pair.first.y) || (p.x > pair.second.x) || (p.y > pair.second.y));
}
void rebdev::inframe(std::istream & in, std::ostream & out, const polyVec & vec)
{
  Polygon poly;
  in >> poly;
  if (poly.points.empty())
  {
    throw std::logic_error("Uncorrect polygon input in INFRAME!");
  }
  std::pair< Point, Point > cornerPoints;
  auto minXVec = (*std::min_element(vec.begin(), vec.end(), std::bind(minXYVec, _1, _2, getX))).points;
  cornerPoints.first.x = (*std::min_element(minXVec.begin(), minXVec.end(), std::bind(minXY, _1, _2, getX))).x;
  auto maxXVec = (*std::max_element(vec.begin(), vec.end(), std::bind(maxXYVec, _1, _2, getX))).points;
  cornerPoints.second.x = (*std::max_element(maxXVec.begin(), maxXVec.end(), std::bind(minXY, _1, _2, getX))).x;
  auto minYVec = (*std::min_element(vec.begin(), vec.end(), std::bind(minXYVec, _1, _2, getY))).points;
  cornerPoints.first.y = (*std::min_element(minYVec.begin(), minYVec.end(), std::bind(minXY, _1, _2, getY))).y;
  auto maxYVec = (*std::max_element(vec.begin(), vec.end(), std::bind(maxXYVec, _1, _2, getY))).points;
  cornerPoints.second.y = (*std::max_element(maxYVec.begin(), maxYVec.end(), std::bind(minXY, _1, _2, getY))).y;
  bool outFrame = std::count_if(poly.points.begin(), poly.points.end(), std::bind(pointOutFrame, _1, cornerPoints));
  out << '<' << ((outFrame == false) ? "TRUE" : "FALSE") << ">\n";
}
