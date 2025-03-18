#include "myFunc.hpp"

void rk2(void f (double t, double y[], double yp[]), double y[], double t, double h)
{
  double k1[2];
  f(t, y, k1);
  double z23[2];
  for (int i = 0; i < 2; ++i)
  {
    z23[i] = y[i] + ((2.0 / 3.0) * h) * k1[i];
  }

  double k2[2];
  f(t + (2.0 / 3.0) * h, z23, k2);
  for (int i = 0; i < 2; ++i)
  {
    y[i] = y[i] + (h / 4.0) * (k1[i] + 3.0 * k2[i]);
  }
}
