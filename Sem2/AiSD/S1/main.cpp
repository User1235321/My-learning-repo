#include <iostream>
#include <utility>

#include "listFunction.hpp"

int main()
{
  rebdev::pairList listOfPairs;
  try
  {
    using namespace rebdev;
    inputList(std::cin, listOfPairs);
    nameAndListOfNumList pair{createNameList(listOfPairs), createListOfNumList(listOfPairs)};
    outputList(std::cout, pair);
  }
  catch (const std::exception & e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }

  return 0;
}
