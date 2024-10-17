#include <iostream>
#include <limits>

int main()
{
  long long int limit = 0;
  std::cin >> limit;
  
  if(!std::cin)
  {
    std::cerr << "Input error!\n";
    return 1;
  }
  long long int num = 1;
  
  while ((num <= (std::numeric_limits< long long int >::max()/2))
  && (num <= limit))
  {
    std::cout << num << '\n';
    num *= 2;
  }
  return 0;
}
