#include "fmin.hpp"
#include <cmath>

static void isNeededGoldenSection(double (*user_func)(double), int label);

static double C = 0.5 * (3.0 - std::sqrt(5.0));
static double EPS = 1.0;
static double A = 0.0;
static double B = 0.0;
static double D = 0.0;
static double E = 0.0;
static double XM = 0.0;
static double P = 0.0;
static double Q = 0.0;
static double R = 0.0;
static double TOL1 = 0.0;
static double TOL2 = 0.0;
static double U = 0.0;
static double V = 0.0;
static double W = 0.0;
static double FU = 0.0;
static double FV = 0.0;
static double FW = 0.0;
static double FX = 0.0;
static double X = 0.0;

double fMin(double ax, double bx, double (*user_func)(double), double tol)
{
  do
  {
    EPS /= 2.0;
    TOL1 = 1.0 + EPS;
  }
  while (TOL1 > 1.0);
  EPS = std::sqrt(EPS);

  A = ax;
  B = bx;
  V = A + C * (B - A);
  W = V;
  X = V;
  E = 0.0;
  FX = user_func(X);
  FV = FX;
  FW = FX;

  while (true)
  {
    XM = 0.5 * (A + B);
    TOL1 = EPS * std::fabs(X) + tol / 3.0;
    TOL2= 2.0 * TOL1;

    if (std::fabs(X - XM) <= (TOL2 - 0.5 * (B - A)))
    {
      return X;
    }

    if (std::fabs(E) <= TOL1)
    {
      isNeededGoldenSection(user_func, 40);
      continue;
    }

    R = (X - W) * (FX - FV);
    Q = (X - V) * (FX - FW);
    P = (X - V) * Q - (X - W) * R;
    Q = 2.0 * (Q - R);
    if(Q > 0.0) P = -P;
    Q = std::fabs(Q);
    R = E;
    E = D;

    if ((std::fabs(P) >= std::fabs(0.5*Q*R)) ||
        (P <= Q * (A - X))                   ||
        (P >= Q * (B - X))                    )
    {
      isNeededGoldenSection(user_func, 40);
      continue;
    }

    D = P / Q;
    U = X + D;

    if (((U - A) < TOL2) || ((B - U) < TOL2))
        D = (XM - X) >= 0 ? std::fabs(TOL1) : -std::fabs(TOL1);
    isNeededGoldenSection(user_func, 50);
    continue;
  }
}

static void isNeededGoldenSection(double (*user_func)(double), int label)
{
  if (label == 40)
  {
    X >= XM ? E = A - X : E = B - X;
    D = C * E;
  }

  if (std::fabs(D) >= TOL1)
      U = X + D;
  else
      U = X + (D >= 0 ? std::fabs(TOL1) : -std::fabs(TOL1));

  FU = user_func(U);

  if (FU > FX)
  {
    U < X ? A = U : B = U;

    if ((FU <= FW) || (W == X))
    {
      V = W;
      FV = FW;
      W = U;
      FW = FU;

      return;
    }

    if ((FU <= FV) || (V == X) || (V == W))
    {
      V = U;
      FV = FU;

      return;
    }

    return;
  }

  U >= X ? A = X : B = X;
  V = W;
  FV = FW;
  W = X;
  FW = FX;
  X = U;
  FX = FU;

  return;
}
