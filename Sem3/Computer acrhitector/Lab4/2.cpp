/*
  I can't write universal function, which use assembler insertion with different commands.

  Example:
  void myFunction(std::string str)
  {
    __asm__(str);
  }
  int main()
  {
    std::string str = "addl %ebx, %eax\n\t"
    myFunction(str)
  }
  Or
  int main()
  {
    myFunction("addl %ebx, %eax\n\t")
  }

  So, i know, that macros is very bad, but in this situation i haven't choice.
  P.s. don't burn my curtains please
*/
#include <iostream>
#include <chrono>

#define mainFunction(value) \
  start = timer1.now(); \
  for (size_t i = 0; i < 100; ++i) \
  { \
    __asm__ \
    ( \
      value \
      value \
      value \
      value \
      value \
      value \
      value \
      value \
      value \
      value \
    ); \
  } \
  end = timer1.now(); \
  std::cout << std::chrono::duration_cast< std::chrono::nanoseconds >(end - start).count() << " nanoseconds\n"; \
  __asm__ \
  ( \
    "movl %0, %%eax\n\t" \
    :"=m"(ax) \
  )

int main()
{
  std::chrono::steady_clock timer1;
  auto start = timer1.now();
  auto end = timer1.now();
  int * ax = new int;
  *ax = 0;

  std::cout << "add eax, 12 (addl $12, \%eax) 10 x 100 times: ";
  mainFunction("addl $12, %eax\n\t");

  std::cout << "add eax, ebx (addl \%ebx, \%eax) 10 x 100 times: ";
  int * bx = new int[11];
  bx[0] = 15;
  __asm__
  (
    "movl %0, %%ebx\n\t"
    :"=m"(bx)
  );//if i just try to use "addl (%ebx), %eax", without change ebx, programm is crash
  mainFunction("addl %ebx, %eax\n\t");

  std::cout << "add eax, [ebx] (addl (\%ebx), \%eax) 10 x 100 times: ";
  mainFunction("addl (%ebx), %eax\n\t");

  std::cout << "add eax, [ebx + 40] (addl 40(\%ebx), \%eax) 10 x 100 times: ";
  mainFunction("addl 40(%ebx), %eax\n\t");

  std::cout << "add [ebx], eax (addl %eax, (\%ebx)) 10 x 100 times: ";
  mainFunction("addl %eax, (%ebx)\n\t");

  delete ax;
  delete[] bx;
  return 0;
}
