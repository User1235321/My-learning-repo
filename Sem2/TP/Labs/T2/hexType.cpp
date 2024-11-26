#include "hexType.hpp"

#include <delimeter.hpp>

#include "streamGuard.hpp"

std::istream & rebdev::operator>>(std::istream & in, HexTypeIO && hex)
{
  std::istream::sentry sentryGuard(in);
  if (!sentryGuard)
  {
    return in;
  }
  StreamGuard guard(in);

  in >> Delimeter{'0'} >> Delimeter{'x'} >> std::hex >> hex.data;
  return in;
}

std::ostream & rebdev::operator<<(std::ostream & out, HexTypeIO && hex)
{
  std::ostream::sentry sentryGuard(out);
  if (!sentryGuard)
  {
    return out;
  }
  StreamGuard guard(out);

  out << "0x" << std::uppercase << std::hex << hex.data;

  return out;
}
