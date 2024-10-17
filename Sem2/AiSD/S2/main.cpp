#include <iostream>
#include <fstream>
#include <string>

#include "stack.hpp"
#include "postfix.hpp"

int main(int argv, char ** argc)
{
  std::ifstream inFile;
  if (argv == 2)
  {
    inFile.open(argc[1]);
    if (inFile.eof())
    {
      std::cout << '\n';
      return 0;
    }
  }
  std::istream & in = (inFile.is_open() ? inFile : std::cin);
  rebdev::Stack< long long > resStack;
  while (!in.eof())
  {
    std::string inStr;
    std::getline(in, inStr);
    if (inStr.empty())
    {
      continue;
    }
    rebdev::postFixQueue postFix;

    try
    {
      rebdev::makePostFix(inStr, postFix);
      if (postFix.empty())
      {
        break;
      }
      long long num = rebdev::postFixToResult(postFix);
      resStack.push(num);
    }
    catch (const std::exception & e)
    {
      std::cerr << e.what() << '\n';
      return 1;
    }
  }
  if (!resStack.empty())
  {
    for (size_t i = 0; i < (resStack.size() - 1); ++i)
    {
      std::cout << *(resStack.drop()) << ' ' ;
    }
    std::cout << *(resStack.drop());
  }
  std::cout << '\n';
  return 0;
}
