#ifndef POINT_H
#define POINT_H
#include <iostream>
class Point
{   
  public:
    Point();
    Point(double x, double y);
    Point(Point & point);
    ~Point();
    
    bool operator==(const Point & point) const;
    bool operator<(const Point & point) const;
    bool operator>=(const Point & point) const;
    Point operator+(double k);
    Point & operator+=(double k);
    
    void setX(double x);
    void setY(double y);
    double getX() const;
    double getY() const;

    bool isEqual(Point point);
    double getDistance(Point point);
    void move(double distance);
  private:
    double x_, y_;
};

#endif
