#include "arrayFunction.hpp"
#include <iostream>

void inArray (int * arr, size_t size)
{
  for (size_t i = 0; i < size; ++i)
  {
    std::cin >> arr[i];
    if (!std::cin)
    {
      throw std::logic_error("Input error!");
    }
  }
};

void outArray (int * arr, size_t size)
{
  for (size_t i = 0; i < size; ++i)
  {
    std::cout << arr[i] << " ";
  }
};

void permutationArray (int * arr, size_t size)
{
  for(size_t i = 0; i < (size - 1); i += 2)
  {
    arr[i] += arr[i + 1];
    arr[i + 1] = arr[i] - arr[i + 1];
    arr[i] -= arr[i + 1];
  }
};
