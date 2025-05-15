#include "matrixFunction.hpp"
#include <iostream>

void inArray (int * matrix, size_t rows, size_t colums)
{
  for (size_t i = 0; i < (rows * colums); ++i)
  {
    std::cin >> matrix[i];
    if (!std::cin)
    {
      throw std::logic_error("Input error!");
    }
  }
};

void outArray (int * matrix, size_t rows, size_t colums)
{
  for (size_t i = 0; i < rows; ++i)
  {
    for (size_t j = 0; j < colums; ++j)
    {
      std::cout << matrix[i * colums + j] << " ";
    }
    std::cout << '\n';
  }
};

size_t getNumberOfOrderedRows (int * matrix, size_t rows, size_t colums)
{
  size_t num = 0;
  for (size_t i = 0; i < rows; ++i)
  {
    bool prot = 1;
    for (size_t j = 0; j < (colums - 1); ++j)
    {
      if (matrix[i * colums + j] >= matrix[i * colums + j + 1])
      {
        prot = 0;
        break;
      }
    }
    num += prot;
  }
  
  return num;
};
