#include <iostream>
#include <cmath>
#include <vector>

#include "spline.h"

const unsigned pointsNum = 10;

double baseFunc(double x)
{
  return (1 - std::exp(x));
}
double LagrangePolynomial(const std::vector< double > & basePoints, double x)
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
double splineNum(const std::vector< double > & basePoints, double x)
{
  std::vector< double > func(pointsNum);
  for (size_t i = 0; i < pointsNum; ++i)
  {
    func[i] = baseFunc(basePoints[i]);
  }
  tk::spline s(basePoints, func);
  return s(x);
}

int main()
{
  std::cout << "\033[1;32m";
  std::cout << "Base points = 0,3 * k (k = 0, 1, ...10)\nf(x) = 1 - exp(x)\nPoints to calculate = 0,15 + 0,3k (k = 0, 1, ...9)\n";
  std::cout << "\033[0m\n";

  std::vector< double > startPoints(pointsNum);
  for (size_t i = 0; i <= pointsNum; ++i)
  {
    startPoints[i] = (0.3d * i);
  }

  for (size_t i = 0; i < pointsNum; ++i)
  {
    double x = 0.15d + 0.3d * i;
    std::cout << "x = " << x;
    std::cout << "; f(x) = " << baseFunc(x);
    std::cout << "; L(x) = " << LagrangePolynomial(startPoints, x);
    std::cout << "; S(x) = " << splineNum(startPoints, x) << '\n';
  }

  std::cout << '\n';
  return 0;
}
