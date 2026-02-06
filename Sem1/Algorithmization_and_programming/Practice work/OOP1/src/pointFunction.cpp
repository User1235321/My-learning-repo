#include "pointFunction.hpp"
#include <iostream>
#include <cmath>

bool isPointInCircle(const Point & point, size_t radius)
{
  double x = point.getX(), y = point.getY();
  return std::sqrt(x * x + y * y) <= radius;
};

void input (Point & point)
{
  double x = 0.0, y = 0.0;
  std::cin >> x >> y;
  if (!std::cin)
  {
    throw std::logic_error("Bad input!");
  }
  point.setX(x);
  point.setY(y);
};

void output (const Point & point)
{
  std::cout << point.getX() << " ";
  std::cout << point.getY() << " ";
};
