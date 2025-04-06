#include <iostream>

#include "first.hpp"
#include "second.hpp"
#include "third.hpp"

int main()
{
  mmx(std::cout);
  sse(std::cout);
  avx(std::cout);
  std::cout << "\n\n";
  m64(std::cout);
  m128(std::cout);
  m256(std::cout);
  std::cout << "\n\n";
  vec16c(std::cout);
  vec4f(std::cout);
  vec8f(std::cout);
  return 0;
}
