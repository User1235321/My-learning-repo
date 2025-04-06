#ifndef MYFUNC_HPP
#define MYFUNC_HPP

#include <iostream>

template < class T >
void print(std::ostream & out, size_t rows, size_t colums, T * arr)
{
  for (size_t i = 0; i < colums; ++i)
  {
    for (size_t j = 0; j < rows; ++j)
    {
      out << (long double)arr[i * rows + j] << '\t';
    }
    out << '\n';
  }
}

#endif
