#include <iostream>
#include <cstddef>
#include "arrayFunction.hpp"

int main()
{
  size_t size = 0;
  std::cin >> size;
  if (!std::cin)
  {
    std::cerr << "Incorrect size of array! \n";
    return 1;
  }
  
  int * arr = nullptr;
  try
  {
    arr =  new int[size];
  }
  catch (const std::exception & e)
  {
    delete [] arr;
    std::cerr << e.what();
    return 2;
  }

  try
  {
    inArray(arr, size);
  }
  catch (const std::exception & e)
  {
    delete [] arr;
    std::cerr << e.what();
    return 1; 
  }

  permutationArray(arr, size);
  outArray(arr, size);

  delete [] arr;
  return 0;
}
