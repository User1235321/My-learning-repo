#include "stringType.hpp"

#include <iomanip>

#include <delimeter.hpp>
#include "streamGuard.hpp"

std::istream & rebdev::operator>>(std::istream & in, StringTypeIO && str)
{
  std::istream::sentry sentryGuard(in);
  if(!sentryGuard)
  {
    return in;
  }
  StreamGuard guard(in);

  in >> Delimeter{'\"'};
  std::getline(in, str.data, '\"');

  return in;
}

std::ostream & rebdev::operator<<(std::ostream & out, StringTypeIO && str)
{
  std::ostream::sentry sentryGuard(out);
  if (!sentryGuard)
  {
    return out;
  }
  StreamGuard guard(out);

  out << std::quoted(str.data, '\"');

  return out;
}
