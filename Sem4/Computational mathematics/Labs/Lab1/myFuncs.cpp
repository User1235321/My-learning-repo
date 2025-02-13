#include "myFuncs.hpp"

#include <cmath>

#include "spline.h"

double baseFunc(double x)
{
  return (1 - std::exp(x));
}
double LagrangePolynomial(const unsigned pointsNum, const std::vector< double > & basePoints, double x)
{
  double result = 0;
  for (size_t i = 0; i < pointsNum; ++i)
  {
    double localResult = baseFunc(basePoints[i]);
    for (size_t j = 0; j < pointsNum; ++j)
    {
      if (j != i)
      {
        localResult *= (x - basePoints[j]);
        localResult /= (basePoints[i] - basePoints[j]);
      }
    }
    result += localResult;
  }
  return result;
}
double splineNum(const unsigned pointsNum, const std::vector< double > & basePoints, double x)
{
  std::vector< double > func(pointsNum);
  for (size_t i = 0; i < pointsNum; ++i)
  {
    func[i] = baseFunc(basePoints[i]);
  }
  tk::spline s(basePoints, func);
  return s(x);
}
