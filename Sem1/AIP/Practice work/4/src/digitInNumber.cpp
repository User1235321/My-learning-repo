#include "digitInNumber.hpp"
#include <stdexcept>

bool isDigitalInNumber (unsigned int digital, long long int number)
{
  if (number < 0)
  {
    number *= -1;
  }
  if (digital > 9)
  {
    throw std::invalid_argument ("Digital can't be more then 9");
  }
  while (number != 0)
  {
    if ((number % 10) == digital)
    {
      return 1;
    }
    number /= 10;
  }
  return 0;
}
