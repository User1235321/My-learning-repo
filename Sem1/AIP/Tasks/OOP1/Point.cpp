#include "Point.h"
#include <cmath>
#include <iostream>

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
