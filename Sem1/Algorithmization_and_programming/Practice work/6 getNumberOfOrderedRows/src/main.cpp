#include <iostream>
#include <cstddef>
#include "matrixFunction.hpp"

int main()
{
  size_t rows = 0, colums = 0;
  std::cin >> rows >> colums;
  if (!std::cin)
  {
    std::cerr << "Incorrect input of rows or colums! \n";
    return 1;
  }
  
  int * matrix = nullptr;
  try
  {
    matrix =  new int[rows * colums];
  }
  catch (const std::exception & e)
  {
    delete [] matrix;
    std::cerr << e.what();
    return 2;
  }
  size_t standartRows = 3, standartColums = 3;
  int standartMatrix[] = {
  1, 2, 3,
  6, 5, 4,
  7, 8, 9
  };
  
  try
  {
    inArray(matrix, rows, colums);
  }
  catch (const std::exception & e)
  {
    delete [] matrix;
    std::cerr << e.what();
    return 1; 
  }

  
  std::cout << "Dynamic: " << getNumberOfOrderedRows(matrix, rows, colums) << '\n';
  std::cout << "Standart: " << getNumberOfOrderedRows(standartMatrix, standartRows, standartColums) << '\n';
  delete [] matrix;
  return 0;
}
