#include "delimeter.hpp"

#include <cctype>

std::istream & rebdev::operator>>(std::istream & in, Delimeter && exp)
{
  std::istream::sentry guard(in);
  if (!guard)
  {
    return in;
  }

  char c = 0;
  in >> c;

  c = std::tolower(c);
  char lowExpected = std::tolower(exp.expected);

  if (c != lowExpected)
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}
