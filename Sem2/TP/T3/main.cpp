#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <fstream>
#include <map>
#include <string>
#include <limits>
#include <functional>
#include <iomanip>
#include <exception>

#include "polygon.hpp"
#include "commands.hpp"

bool notEmpty(const rebdev::Polygon & poly)
{
  return !poly.points.empty();
}
int main(int argc, char ** argv)
{
  if (argc != 2)
  {
    std::cerr << "I work only with one programm argumet!";
    return 1;
  }
  std::ifstream inFile(argv[1]);
  using inputItT = std::istream_iterator< rebdev::Polygon >;
  using polyVec = std::vector< rebdev::Polygon >;
  polyVec inVec;
  inVec.insert(inVec.begin(), inputItT{ inFile }, inputItT{});
  size_t elementNum = std::count_if(inVec.begin(), inVec.end(), notEmpty);
  polyVec polygonsVector(elementNum);
  std::copy_if(inVec.begin(), inVec.end(), polygonsVector.begin(), notEmpty);
  std::map< std::string, std::function< void(std::istream &, std::ostream &, const polyVec &) > > commandMap;
  commandMap["AREA"] = rebdev::areaBase;
  commandMap["MAX"] = rebdev::maxBase;
  commandMap["MIN"] = rebdev::minBase;
  commandMap["COUNT"] = rebdev::countBase;
  commandMap["RECTS"] = rebdev::rects;
  commandMap["INFRAME"] = rebdev::inframe;
  std::string inStr;
  std::cout << std::fixed << std::setprecision(1);
  while (std::cin >> inStr)
  {
    try
    {
      commandMap.at(inStr)(std::cin, std::cout, polygonsVector);
    }
    catch (const std::out_of_range & e)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    catch (const std::exception & e)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }
  return 0;
}
