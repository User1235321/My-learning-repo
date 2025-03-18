#include <iostream>
#include <cmath>
#include <iomanip>
#include <limits>//dell

#include "rkf45.hpp"
#include "myFunc.hpp"

void f (double t, double x[], double xp[])
{
  xp[0] = -14.0 * x[0] + 13.0 * x[1] + std::cos(1.0 + t);
  xp[1] = 20.0 * x[0] - 30.0 * x[1] + std::atan(1.0 + t * t);
}

int main()
{
  std::cout <<  std::fixed << std::setprecision(6);

  double eps = 0.0001;
  double xStart[2] = {2, 0.5};
  double xpStart[2] = {0, 0};
  double tStart = 0;
  double standartStep = 0.075;
  double t = 0;

  f(0, xStart, xpStart);
  std::cout << "\033[1;32mStarting values:\033[1;0m\nx1(0) = " << xStart[0] << "; x2(0) = " << xStart[1] << "\ndx1/xdt(0) = " << xpStart[0] << "; dx2/xdt(0) = " << xpStart[1] << "\n\n\033[1;32mrkf45:\033[1;0m\n";
  for (t = 0; t < 1.5; t += standartStep)
  {
    r8_rkf45(f, 2, xStart, xpStart, &tStart, t, &eps, eps, 1);
    std::cout << "t = " << t << ";\tx1 = " << xStart[0] << ";\tx2 = " << xStart[1] << '\n';
  }

  std::cout << "\n\033[1;32mrk2 with 0.075 step:\033[1;0m\n";
  xStart[0] = 2;
  xStart[1] = 0.5;
  for (t = standartStep; t < 1.51; t += standartStep)
  {
    rk2(f, xStart, t, standartStep);
    std::cout << "t = " << t << ";\tx1 = " << xStart[0] << ";\tx2 = " << xStart[1] << '\n';
  }

  double bestStep = 0.05;
  std::cout << "\n\033[1;32mrk2 with optimal step (0.05):\033[1;0m\n";
  xStart[0] = 2;
  xStart[1] = 0.5;
  for (t = bestStep; t < 1.51; t += bestStep)
  {
    rk2(f, xStart, t, bestStep);
    std::cout << "t = " << t << ";\tx1 = " << xStart[0] << ";\tx2 = " << xStart[1] << '\n';
  }

  std::cout << "\n\033[1;32mrkf45 with 0.05 step:\033[1;0m\n";
  xStart[0] = 2;
  xStart[1] = 0.5;
  tStart = 0;
  f(0, xStart, xpStart);
  for (t = 0; t < 1.5; t += bestStep)
  {
    r8_rkf45(f, 2, xStart, xpStart, &tStart, t, &eps, eps, 1);
    std::cout << "t = " << t << ";\tx1 = " << xStart[0] << ";\tx2 = " << xStart[1] << '\n';
  }

  //dell
  bestStep = 0.001;
  double xMin = std::numeric_limits< double >::max(), xMax = std::numeric_limits< double >::min();
  std::cout << "\n\033[1;32mrk2 with optimal step (0.05):\033[1;0m\n";
  double xStart2[2] = {2, 0.5};
  xStart[0] = 2;
  xStart[1] = 0.5;
  tStart = 0;
  f(0, xStart, xpStart);
  t = 0;
  r8_rkf45(f, 2, xStart, xpStart, &tStart, t, &eps, eps, 1);

  for (t = bestStep; t < 1.51; t += bestStep)
  {
    rk2(f, xStart, t, bestStep);
    r8_rkf45(f, 2, xStart2, xpStart, &tStart, t, &eps, eps, 1);
    xMin = std::min(xMin, std::min(std::abs(std::abs(xStart2[0]) - std::abs(xStart[0])), std::abs(std::abs(xStart2[1]) - std::abs(xStart[1]))));
    xMax = std::max(xMax, std::max(std::abs(std::abs(xStart2[0]) - std::abs(xStart[0])), std::abs(std::abs(xStart2[1]) - std::abs(xStart[1]))));
  }
  std::cout << "min: " << xMin << "; max: " << xMax << '\n';
  //dell
  return 0;
}
