#include <iostream>
#include <iomanip>
#include <vector>

#include "matrixFunc.hpp"

int main()
{
  size_t n[] = {3, 6, 9};

  std::cout << std::fixed << std::setprecision(8);
  for (size_t q = 0; q < 3; ++q)
  {
    std::vector< double > b(n[q] * n[q]);
    fillMatrix(b);
    std::cout << "Standart matrix " << n[q] << 'x' << n[q] << ":\n";
    printMatrix(b, std::cout);

    std::vector< double > antiB(antiMatrix(b));
    std::cout << "Anti matrix:\n";
    printMatrix(antiB, std::cout);

    std::vector< double > r(multiMatrix(b, antiB));
    for (int i = 0; i < n[q]; ++i)
    {
      r[i * n[q] + i] -= 1;
    }
    std::cout << "R matrix:\n";
    printMatrix(r, std::cout);
    std::cout << "Norm of R: " << calcNorm(r) << "\n\n";
  }
  return 0;
}
