#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>

#include "myFuncs.hpp"
#include "paint.hpp"

const unsigned pointsNum = 10;

int main()
{
  std::cout.precision(8);
  std::cout << "\033[1;32m";
  std::cout << "Base points = 0,3 * k (k = 0, 1, ...10)\nf(x) = 1 - exp(x)\nPoints to calculate = 0,15 + 0,3k (k = 0, 1, ...9)\n";
  std::cout << "\033[0m\n";

  std::vector< double > x(pointsNum);

  for (size_t i = 0; i <= pointsNum; ++i)
  {
    x[i] = (0.3d * i);
  }

  std::vector< double > orig(pointsNum);
  std::vector< double > Lagrange(pointsNum);
  std::vector< double > spline(pointsNum);

  for (size_t i = 0; i < pointsNum; ++i)
  {
    double point = 0.15d + 0.3d * i;
    std::cout << "x = " << point;

    orig[i] = baseFunc(point);
    std::cout << "; f(x) = " << orig[i];

    Lagrange[i] = LagrangePolynomial(pointsNum, x, point);
    std::cout << "; L(x) = " << Lagrange[i];

    spline[i] = splineNum(pointsNum, x, point);
    std::cout << "; S(x) = " << spline[i] << '\n';
  }

  std::cout << '\n';

  paint(orig, Lagrange, spline);
  return 0;
}
