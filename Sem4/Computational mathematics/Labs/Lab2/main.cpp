#include <iostream>
#include <iomanip>

int main()
{
  size_t n[] = {3, 6, 9};
  double ** b = new double * [3];
  double ** antiB = new double * [3];
  double ** r = new double * [3];

  std::cout << std::fixed << std::setprecision(8);;
  for (size_t q = 0; q < 3; ++q)
  {
    b[q] = new double[3 * 3 * (q + 1) * (q + 1)];
    antiB[q] = new double[3 * 3 * (q + 1) * (q + 1)];
    r[q] = new double[3 * 3 * (q + 1) * (q + 1)];

    for (int i = 0; i < n[q]; ++i)
    {
      for (int j = 0; j < n[q]; ++j)
      {
        if (i == j)
        {
          b[q][i * n[q] + j] = 0.01 / ((n[q] - i + j) * (i + 1));
        }
        else if (i < j)
        {
          b[q][i * n[q] + j] = 0;
        }
        else
        {
          b[q][i * n[q] + j] = i * (n[q] - j);
        }
        std::cout << b[q][i * n[q] + j] << '\t';
      }
      std::cout << '\n';
    }
    std::cout << "\n\n";
  }
  for (size_t q = 0; q < 3; ++q)
  {
    r[q] = multiMatrix(b[q], antiMatrix(b[q]));
    for (int i = 0; i < n[q]; ++i)
    {
      r[q][i * n[q] + i] -= 1;
    }
    std::cout << (q + 1) << "Matrix norm = " << calcNorm(r[q]);
  }


  for (size_t q = 0; q < 3; ++q)
  {
    delete[] b[q];
    delete[] antiB[q];
    delete[] r[q];
  }
  delete[] b;
  delete[] antiB;
  delete[] r;
  return 0;
}
