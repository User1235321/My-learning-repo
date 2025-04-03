#include "first.hpp"

#include <iostream>

template < class T >
void print(std::ostream & out, size_t rows, T * arr)
{
  for (size_t i = 0; i < 2; ++i)
  {
    for (size_t j = 0; j < rows; ++j)
    {
      out << (long long)arr[i * rows + j] << ' ';
    }
    out << '\n';
  }
}
void mmx(std::ostream & out)
{
  unsigned char mmx[2][8] = {{1, 2, 3, 4, 5, 6, 7, 8}, {9, 11, 13, 15, 17, 19, 21, 23}}, flags = 0;
  out << "\033[1;32mMMX adding:\033[0m\nBefore adding:\n";
  print< unsigned char >(out, 8, &mmx[0][0]);
  __asm__
  (
    "movq %1, %%mm0\n"
    "movq %2, %%mm1\n"
    "paddb %%mm1, %%mm0\n"
    "movq %%mm0, %0\n"
    "emms\n"
    :"=m"(mmx[1]) // Output operand
    :"m"(mmx[0]), "m"(mmx[1])  // Input operands
    :"mm0", "mm1" // Clobbered registers
  );
  out << "After adding:\n";
  print< unsigned char >(out, 8, &mmx[0][0]);
}
void sse(std::ostream & out)
{
  float sse[2][4] = {{1, 2, 3, 4}, {5, 6, 7, 8}};
  out << "\n\033[1;32mSSE mul:\033[0m\nBefore mul:\n";
  print< float >(out, 4, &sse[0][0]);
  __asm__
  (
    "movups %1, %%xmm0\n"
    "movups %2, %%xmm1\n"
    "mulps %%xmm0, %%xmm1\n"
    "movups %%xmm1, %0\n"
    :"=m"(sse[1]) // Output operand
    :"m"(sse[0]), "m"(sse[1])  // Input operands
    :"xmm0", "xmm1" // Clobbered registers
  );
  out << "After mul:\n";
  print< float >(out, 4, &sse[0][0]);
}
void avx(std::ostream & out)
{
  float avx[2][8] = {{10, 20, 30, 40, 50, 60, 70, 80}, {1, 2, 3, 4, 5, 6, 7, 8}};
  out << "\n\033[1;32mAVX :\033[0m\nBefore mul:\n";
  print< float >(out, 8, &avx[0][0]);
  __asm__
  (
    "vmovups %1, %%ymm0\n"
    "vmovups %2, %%ymm1\n"
    "vdivps %%ymm1, %%ymm0, %%ymm2\n"
    "vmovups %%ymm2, %0\n"
    :"=m"(avx[1]) // Output operand
    :"m"(avx[0]), "m"(avx[1])  // Input operands
    :"ymm0", "ymm1", "ymm2" // Clobbered registers
  );
  out << "After mul:\n";
  print< float >(out, 8, &avx[0][0]);
}
