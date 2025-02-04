#include <iostream>
#include <cmath>

#include "spline.hpp"//dell

long double baseFunc(long double x)
{
  return (1 - std::exp(x));
}
long double LagrangePolynomial(size_t arrSize, long double * basePoints, long double x)
{
  long double result = 0;
  for (size_t i = 0; i < arrSize; ++i)
  {
    long double localResult = baseFunc(basePoints[i]);
    for (size_t j = 0; j < arrSize; ++j)
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
long double splineNum()
{

}

int main()
{
  std::cout << "\033[1;32m";
  std::cout << "Base points = 0,3 * k (k = 0, 1, ...10)\nf(x) = 1 - exp(x)\nPoints to calculate = 0,15 + 0,3k (k = 0, 1, ...9)\n";
  std::cout << "\033[0m\n";

  const size_t pointsNum = 10;
  long double startPoints[pointsNum];
  for (size_t i = 0; i <= pointsNum; ++i)
  {
    startPoints[i] = (0.3d * i);
  }

  for (size_t i = 0; i <= 9; ++i)
  {
    long double x = 0.15d + 0.3d * i;
    std::cout << "x = " << x;
    std::cout << "; f(x) = " << baseFunc(x);
    std::cout << "; L(x) = " << LagrangePolynomial(pointsNum, startPoints, x);
    std::cout << "; f(x) - L(x) = " << (baseFunc(x) - LagrangePolynomial(pointsNum, startPoints, x)) << '\n';
  }
  return 0;
}
