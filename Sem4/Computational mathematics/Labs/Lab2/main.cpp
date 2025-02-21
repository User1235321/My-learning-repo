#include <iostream>
#include <iomanip>
#include <vector>

#include "matrixFunc.hpp"

int main()
{
  size_t n[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

  for (size_t q = 0; q < 11; ++q)
  {
    std::vector< double > b(n[q] * n[q]);
    fillMatrix(b);
    std::cout << "\033[1;32mStandart matrix " << n[q] << 'x' << n[q] << ":\033[1;0m\n";
    printMatrix(b, std::cout);

    std::vector< double > antiB(antiMatrix(b));
    std::cout << "\033[1;33mAnti matrix:\033[1;0m\n";
    printMatrix(antiB, std::cout);

    std::vector< double > r(multiMatrix(b, antiB));
    for (int i = 0; i < n[q]; ++i)
    {
      r[i * n[q] + i] -= 1;
    }
    std::cout << "\033[1;34mR matrix:\033[1;0m\n";
    printMatrix(r, std::cout);
    std::cout << "\033[1;35mNorm of R: " << calcNorm(r) << "\033[1;0m\n\n";
  }
  return 0;
}
