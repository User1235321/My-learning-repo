#include <iostream>
#include <cstddef>
#include <ctime>
#include <limits>
#include "Point.h"
#include "pointFunction.hpp"

int main()
{
  Point * pointArr = nullptr;
  Point * newArr = nullptr;
  size_t numberOfPoints = 0;
  do
  {
    newArr = new Point[numberOfPoints + 1];
    for (size_t i = 0; i < numberOfPoints; ++i)
    {
      newArr[i].setX(pointArr[i].getX());
      newArr[i].setY(pointArr[i].getY());
    }

    try
    {
      input(newArr[numberOfPoints]);
    }
    catch (const std::logic_error & e)
    {
      delete[] pointArr;
      delete[] newArr;

      std::cerr << e.what();
      return 1;
    }

    delete[] pointArr;
    pointArr = newArr;
    newArr = nullptr;

    numberOfPoints += 1;
  } while ((pointArr[numberOfPoints - 1].getX() != 0.0) || (pointArr[numberOfPoints - 1].getY() != 0.0));
  
  srand(time(NULL));
  size_t numberOfFirstPoint = (rand() % (numberOfPoints - 1)), numberOfSecondPoint = (rand() % (numberOfPoints - 1));
  while (numberOfSecondPoint == numberOfFirstPoint)
  {
    numberOfSecondPoint = (rand() % (numberOfPoints - 1));
  }
  std::cout << "Number of points: " << numberOfFirstPoint << " " << numberOfSecondPoint << '\n';

  std::cout << "x and y cordinats: ";
  output(pointArr[numberOfFirstPoint]);
  std::cout << " ";
  output(pointArr[numberOfSecondPoint]);
  std::cout << '\n';

  std::cout << "Points is equal: " << pointArr[numberOfFirstPoint].isEqual(pointArr[numberOfSecondPoint]) << '\n';

  std::cout << "Distance between points: " << pointArr[numberOfFirstPoint].getDistance(pointArr[numberOfSecondPoint]) << '\n';

  size_t radius = (rand() % 100);
  std::cout << "Points is in circle with radius " << radius << ": " << isPointInCircle(pointArr[numberOfFirstPoint], radius);
  std::cout << " " << isPointInCircle(pointArr[numberOfSecondPoint], radius) << '\n';

  Point startPoint(0.0, 0.0);
  size_t IndexOfNearestPoint = 0;
  double minDistance = std::numeric_limits< double >::max();

  for (size_t i = 0; i < (numberOfPoints - 1); ++i)
  {
    double distance = startPoint.getDistance(pointArr[i]);
    if (distance < minDistance)
    {
      minDistance = distance;
      IndexOfNearestPoint = i;
    }
  }
  
  std::cout << "The point closest to the origin: ";
  output(pointArr[IndexOfNearestPoint]);
  std::cout << '\n';

  return 0;
}
