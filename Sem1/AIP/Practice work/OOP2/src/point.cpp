#include "point.h"
#include <cmath>

Point::Point():
  x_(0.0),
  y_(0.0)
  {};
Point::Point(double x, double y):
  x_(x),
  y_(y)
  {};
Point::Point(Point & point):
  x_(point.getX()),
  y_(point.getY())
  {};
Point::~Point()
{
  x_ = 0.0;
};

bool Point::operator==(const Point & point) const
{
  return ((x_ == point.getX()) && (y_ == point.getY()));
};
bool Point::operator<(const Point & point) const
{
  return ((x_ * x_ + y_ * y_) < (point.getX() * point.getX() + point.getY() * point.getY()));
};
bool Point::operator>=(const Point & point) const
{
  return (point < *this);
};
Point Point::operator+(double k)
{
  Point point((x_ + k), (y_ + k));
  return point;
};
Point & Point::operator+=(double k)
{
  x_ += k;
  y_ += k;
  return *this;
};

void Point::setX(double x)
{
  x_ = x;
};
void Point::setY(double y)
{
  y_ = y;
};
double Point::getX() const
{
  return x_;
};
double Point::getY() const
{
  return y_;
};

bool Point::isEqual(Point point)
{
  return (x_ == point.getX()) && (y_ == point.getY());
};
double Point::getDistance(Point point)
{
  return std::sqrt((x_ - point.getX()) * (x_ - point.getX()) + (y_ - point.getY()) * (y_ - point.getY()));
};
void Point::move(double distance)
{
  x_ += distance;
  y_ += distance;
};
