#include <iostream>
#include "Figure.hpp"

int main()
{
  figure firstFigure, secondFigure;
  std::cin >> firstFigure.column
  >> firstFigure.line
  >> secondFigure.column
  >> secondFigure.line;
  
  if (!std::cin)
  {
    std::cout << "Input error: invalid data format\n";
    return 1;
  }
  
  if (firstFigure() && secondFigure())
  {
    std::cout << firstFigure(secondFigure) << '\n';
  }  
  else
  {
    /*std::cout << "firstFigure.column:" << firstFigure.column << '\n';
    std::cout << "firstFigure.line:" << firstFigure.line << '\n';
    std::cout << "secondFigure.column:" << secondFigure.column << '\n';
    std::cout << "secondFigure.line:" << secondFigure.line << '\n';*/
    std::cout << "Input error: incorrect values ​​entered\n";
    return 1;
  }
  return 0;
}
