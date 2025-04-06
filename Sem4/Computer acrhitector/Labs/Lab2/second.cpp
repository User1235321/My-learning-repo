#include "second.hpp"

#include <xmmintrin.h>
#include <mmintrin.h>
#include <immintrin.h>

#include "myFunc.hpp"

void m64(std::ostream & out)
{
  unsigned char ch[2][8] = {{1, 2, 3, 4, 5, 6, 7, 8}, {9, 11, 13, 15, 17, 19, 21, 23}}, res[8];
  out << "\033[1;33mm64 add:\033[0m\nBefore:\n";
  print< unsigned char >(out, 8, 2, &ch[0][0]);

  __m64 first = _mm_set_pi8(ch[0][0], ch[0][1], ch[0][2], ch[0][3], ch[0][4], ch[0][5], ch[0][6], ch[0][7]);
  __m64 second = _mm_set_pi8(ch[1][0], ch[1][1], ch[1][2], ch[1][3], ch[1][4], ch[1][5], ch[1][6], ch[1][7]);
  _mm_stream_pi((__m64*)ch[1], _mm_add_pi8(first, second));
  _mm_empty();

  out << "After:\n";
  print< unsigned char >(out, 8, 2, &ch[0][0]);
}
void m128(std::ostream & out)
{
  float fl[2][4] = {{1, 2, 3, 4}, {5, 6, 7, 8}};
  out << "\033[1;33mm128 multiplication:\033[0m\nBefore:\n";
  print< float >(out, 4, 2, &fl[0][0]);

  __m128 first = _mm_load_ps(fl[0]);
  __m128 second = _mm_load_ps(fl[1]);
  _mm_store_ps(fl[1], _mm_mul_ps(first, second));
  _mm_empty();

  out << "After:\n";
  print< float >(out, 4, 2, &fl[0][0]);
}
void m256(std::ostream & out)
{
  alignas(32) float fl[2][8] = {{10, 20, 30, 40, 50, 60, 70, 80}, {1, 2, 3, 4, 5, 6, 7, 8}};
  out << "\033[1;33mm256 division:\033[0m\nBefore:\n";
  print< float >(out, 8, 2, &fl[0][0]);

  __m256 first = _mm256_load_ps(fl[0]);
  __m256 second = _mm256_load_ps(fl[1]);
  _mm256_store_ps(fl[1], _mm256_div_ps(first, second));
  _mm_empty();

  out << "After:\n";
  print< float >(out, 8, 2, &fl[0][0]);
}
