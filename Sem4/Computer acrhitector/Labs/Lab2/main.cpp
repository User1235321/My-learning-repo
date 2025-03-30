#include <iostream>

int main()
{
  unsigned char mmx1[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  unsigned char mmx2[8] = {8, 7, 6, 5, 4, 3, 2, 1};
  char flags = 0;
  std::cout << "\033[1;32mMMX compare:\033[0m\nBefore compare:\n";
  for (size_t i = 0; i < 8; ++i)
  {
    std::cout << (int)mmx1[i] << ' ' << (int)mmx2[i] << '\n';
  }
  __asm__
  (
    "movq %2, %%mm0\n"
    "movq %3, %%mm1\n"
    "pcmpeqb %%mm1, %%mm0\n"
    "movq %%mm0, %0\n"
    "emms\n"
    "lahf\n"
    "mov %%ah, %1\n"
    :"=m"(mmx2), "=m"(flags)  // Output operand
    :"m"(mmx1), "m"(mmx2)  // Input operands
    :"mm0", "mm1", "ax"  // Clobbered registers
  );
  std::cout << "After compare:\n";
  for (size_t i = 0; i < 8; ++i)
  {
    std::cout << (int)mmx1[i] << ' ' << (int)mmx2[i] << '\n';
  }
  std::cout << "Zero flag: " << (int)((flags >> 6) & 1u) << ' ' << (int)flags <<'\n';
  unsigned char mmxCopy[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  flags = 0;
  std::cout << "Before compare:\n";
  for (size_t i = 0; i < 8; ++i)
  {
    std::cout << (int)mmx1[i] << ' ' << (int)mmxCopy[i] << '\n';
  }
  __asm__
  (
    "movq %2, %%mm0\n"
    "movq %3, %%mm1\n"
    "pcmpeqb %%mm1, %%mm0\n"
    "movq %%mm0, %0\n"
    "emms\n"
    "lahf\n"
    "mov %%ah, %1\n"
    :"=m"(mmxCopy), "=m"(flags)  // Output operand
    :"m"(mmx1), "m"(mmxCopy)  // Input operands
    :"mm0", "mm1", "ax"  // Clobbered registers
  );
  std::cout << "After compare:\n";
  for (size_t i = 0; i < 8; ++i)
  {
    std::cout << (int)mmx1[i] << ' ' << (int)mmxCopy[i] << '\n';
  }
  std::cout << "Zero flag: " << (int)((flags >> 6) & 1u) << ' ' << (int)flags <<'\n';

  flags = 0;
  float sse1[4] = {1.1, 1.2, 1.3, 1.4};
  float sse2[4] = {-1.1, -1.2, -1.3, -1.4};
  std::cout << "\033[1;32mSSE add:\033[0m\nBefore add:\n";
  for (size_t i = 0; i < 4; ++i)
  {
    std::cout << sse1[i] << ' ' << sse2[i] << '\n';
  }
  __asm__
  (
    "movups %2, %%xmm0\n"
    "movups %3, %%xmm1\n"
    "addps %%xmm0, %%xmm1\n"
    "movups %%xmm1, %0\n"
    "lahf\n"
    "mov %%ah, %1\n"
    :"=m"(sse2), "=m"(flags)  // Output operand
    :"m"(sse1), "m"(sse2)  // Input operands
    :"mm0", "mm1", "ax"  // Clobbered registers
  );
  std::cout << "After add:\n";
  for (size_t i = 0; i < 4; ++i)
  {
    std::cout << sse1[i] << ' ' << sse2[i] << '\n';
  }
  std::cout << "Zero flag: " << (int)((flags >> 6) & 1u) << ' ' << (int)flags <<'\n';

  
}
