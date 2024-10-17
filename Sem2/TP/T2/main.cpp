#include <iostream>
#include <iomanip>
#include <algorithm>
#include <list>
#include <limits>
#include <iterator>

#include "dataStruct.hpp"

int main()
{
  std::list< rebdev::DataStruct > data;

  while (!std::cin.eof())
  {
    using inputItT = std::istream_iterator< rebdev::DataStruct >;
    std::copy(inputItT{ std::cin }, inputItT{}, std::back_inserter(data));

    if (!std::cin)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  data.sort();

  using outputItT = std::ostream_iterator< rebdev::DataStruct >;
  std::cout << std::setprecision(1) << std::fixed;
  std::copy(data.cbegin(), data.cend(), outputItT{ std::cout, "\n" });

  return 0;
}
