#include "matrixFunc.hpp"

#include <cmath>

extern "C" {
  int decomp_(int *ndim, int *n, double *a, double *cond, int *ipvt, double *work);
  int solve_(int *ndim, int *n, double *a, double *b, int *ipvt);
}

void fillMatrix(vec & m) noexcept
{
  int n = std::sqrt(m.size());
  for (int i = 0; i < n; ++i)
  {
    for (int j = 0; j < n; ++j)
    {
      if (i == j)
      {
        m[i * n + j] = 0.01 / ((n - i + j) * (i + 1));
      }
      else if (i < j)
      {
        m[i * n + j] = 0;
      }
      else
      {
        m[i * n + j] = i * (n - j);
      }
    }
  }
}
void printMatrix(const vec & m, std::ostream & out)
{
  int n = std::sqrt(m.size());
  for (size_t i = 0; i < n; ++i)
  {
    for (size_t j = 0; j < n; ++j)
    {
      out << m[i * n + j] << '\t';
    }
    out << '\n';
  }
}
vec antiMatrix(const vec & m)
{
  int dim = std::sqrt(m.size());
  vec copy(m);
  double cond;
  std::vector< int > ipvt(dim);
  vec work(dim);

  decomp_(&dim, &dim, copy.data(), &cond, ipvt.data(), work.data());

  vec result(m.size());
  for (int i = 0; i < dim; ++i)
  {
    vec b(dim, 0.0);
    b[i] = 1.0;
    solve_(&dim, &dim, copy.data(), b.data(), ipvt.data());
    for (int j = 0; j < dim; ++j)
    {
      result[i * dim + j] = b[j];
    }
  }
  return result;
}
vec multiMatrix(const vec & first, const vec & second) noexcept
{
  vec result(first.size());
  int n = std::sqrt(first.size());
  for (size_t i = 0; i < n; ++i)
  {
    for (size_t j = 0; j < n; ++j)
    {
      result[i * n + j] = 0;
      for (size_t q = 0; q < n; ++q)
      {
        result[i * n + j] += first[i * n + q] * second[q * n + j];
      }
    }
  }
  return result;
}
double calcNorm(const vec & m)
{
  int n = std::sqrt(m.size());
  double norm = 0;
  for (size_t i = 0; i < n; ++i)
  {
    for (size_t j = 0; j < n; ++j)
    {
      norm += std::pow(m[i * n + j], 2);
    }
  }
  norm = std::sqrt(norm);
  return norm;
}
