#include <iostream>
#include <chrono>
#include <string>

int main()
{
  std::cout << "add ax, bx 10 x 100 times: ";
  std::chrono::steady_clock timer1;
  auto start = timer1.now();
  for (size_t i = 0; i < 100; ++i)
  {
    asm
    (
      "add %ax, %bx \n\t"
      "add %ax, %bx \n\t"
      "add %ax, %bx \n\t"
      "add %ax, %bx \n\t"
      "add %ax, %bx \n\t"
      "add %ax, %bx \n\t"
      "add %ax, %bx \n\t"
      "add %ax, %bx \n\t"
      "add %ax, %bx \n\t"
      "add %ax, %bx \n\t"
    );
  }
  auto end = timer1.now();
  std::cout << std::chrono::duration_cast< std::chrono::microseconds >(end - start).count() << " microseconds\n";

  std::cout << "add ax, [bx] 10 x 100 times: ";
  start = timer1.now();
  for (size_t i = 0; i < 100; ++i)
  {
    asm
    (
      "add %ax, %[bx] \n\t"
      "add %ax, %[bx] \n\t"
      "add %ax, %[bx] \n\t"
      "add %ax, %[bx] \n\t"
      "add %ax, %[bx] \n\t"
      "add %ax, %[bx] \n\t"
      "add %ax, %[bx] \n\t"
      "add %ax, %[bx] \n\t"
      "add %ax, %[bx] \n\t"
      "add %ax, %[bx] \n\t"
    );
  }
  end = timer1.now();
  std::cout << std::chrono::duration_cast< std::chrono::microseconds >(end - start).count() << " microseconds\n";

  return 0;
}
