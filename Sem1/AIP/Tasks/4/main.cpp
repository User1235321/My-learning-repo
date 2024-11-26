#include <iostream>
#include <stdexcept>
#include "digitInNumber.hpp"
int main ()
{
  unsigned int digital = 0;
  long long int number = 0;

  std::cin >> digital;
  if (!std::cin)
  {
    std::cerr << "Input Error! First argument must be digital from 0 to 9" << "\n";
    return 1;
  }

  std::cin >> number;
  if (!std::cin)
  {
    std::cerr << "Input error! Second argument must be number\n";
    return 1;
  }

  try
  {
    std::cout << isDigitalInNumber(digital, number) << '\n';
  }
  catch (const std::invalid_argument & e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
  return 0;
}
