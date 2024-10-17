#include "standartMathOper.hpp"

#include <limits>
#include <numbers>
#include <cmath>
#include <stdexcept>

double rebdev::plus(const double & f, const double & s)
{
  bool overlow = ((f > 0) && (s > 0) && (f > std::numeric_limits< double >::max() - s));
  overlow = overlow || ((f < 0) && (s < 0) && (f < std::numeric_limits< double >::min() - s));
  if (overlow)
  {
    throw std::invalid_argument("overlow as a result of + !");
  }
  return (f + s);
}
double rebdev::minus(const double & f, const double & s)
{
  bool overlow = ((f > 0) && (s < 0) && (f > std::numeric_limits< double >::max() + s));
  overlow = overlow || ((f < 0) && (s > 0) && (f < std::numeric_limits< double >::min() + s));
  if (overlow)
  {
    throw std::invalid_argument("overlow as a result of - !");
  }
  return (f - s);
}
double rebdev::divides(const double & f, const double & s)
{
  if (s == 0)
  {
    throw std::invalid_argument("try to divides to zero!");
  }
  return (f / s);
}
double rebdev::multiplies(const double & f, const double & s)
{
  bool overlow = ((((f > 0) && (s < 0)) || ((f < 0) && (s > 0))) && (f < std::numeric_limits< double >::min() / s));
  overlow = overlow || ((((f > 0) && (s > 0)) || ((f < 0) && (s < 0))) && (f > std::numeric_limits< double >::max() / s));
  if (overlow)
  {
    throw std::invalid_argument("overlow as a result of * !");
  }
  return (f * s);
}
double rebdev::mySqrt(const double & num)
{
  if (num < 0)
  {
    throw std::invalid_argument("uncorrect argument for operation sqrt!");
  }
  return sqrt(num);
}
double rebdev::tg(const double & num)
{
  if ((fmod(num, acos(0.0)) == 0) && (fmod(num, acos(0.0)) != 0))
  {
    throw std::invalid_argument("uncorrect argument for operation tg!");
  }
  return tan(num);
}
double rebdev::ctg(const double & num)
{
  if (fmod(num, acos(0.0)) == 0)
  {
    throw std::invalid_argument("uncorrect argument for operation ctg!");
  }
  return (cos(num) / sin(num));
}
