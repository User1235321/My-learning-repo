#include "dataStruct.hpp"

#include <delimeter.hpp>
#include "streamGuard.hpp"
#include "hexType.hpp"
#include "binType.hpp"
#include "stringType.hpp"

bool rebdev::DataStruct::operator<(const DataStruct & rhs) const
{
  if (key1 == rhs.key1)
  {
    if (key2 == rhs.key2)
    {
      return key3.size() < rhs.key3.size();
    }
    return (key2 < rhs.key2);
  }
  return (key1 < rhs.key1);
}

std::istream & rebdev::operator>>(std::istream & in, DataStruct & data)
{
  std::istream::sentry sentryGuard(in);
  if(!sentryGuard)
  {
    return in;
  }
  StreamGuard guard(in);

  if (!(in >> Delimeter{'('} >> Delimeter{':'}))
  {
    return in;
  }

  for (size_t i = 0; i < 3; ++i)
  {
    char num = 0;
    in >> Delimeter{'k'} >> Delimeter{'e'} >> Delimeter{'y'} >> num;

    switch (num)
    {
      case '1':
        in >> BinTypeIO{data.key1};
        break;

      case '2':
        in >> HexTypeIO{data.key2} >> Delimeter{':'};
        break;

      case '3':
        in >> StringTypeIO{data.key3} >> Delimeter{':'};
        break;

      default:
        return in;
    }
  }

  in >> Delimeter{')'};
  return in;
}
std::ostream & rebdev::operator<<(std::ostream & out, const DataStruct & data)
{
  std::ostream::sentry sentryGuard(out);
  if (!sentryGuard)
  {
    return out;
  }
  StreamGuard guard(out);

  DataStruct structCopy = data;

  out << "(:key1 " << BinTypeIO{structCopy.key1};
  out << ":key2 " << HexTypeIO{structCopy.key2};
  out << ":key3 " << StringTypeIO{structCopy.key3} << ":)";

  return out;
}
