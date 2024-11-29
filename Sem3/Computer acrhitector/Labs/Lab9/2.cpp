#include <iostream>
int main()
{
  std::cout << "\033[1;32m";
  std::cout << "Programm start. Press esc to finish";
  std::cout << "\033[0m\n";
  char scan = 0, asci = 0, status = 0;
  int adress = 0;
  while(true)
  {
    __asm__
    (
//      "push %ds\n"
      "push %rsi\n"
      "push %rdi\n"
      "mov $0x40, %rax\n"
      "mov %rax, %rds\n"
      "mov $0x1A, %rsi\n"
      "mov $0x1C, %rdi\n"
    );
    wait_kbd:
      __asm__
      (
        "mov (%%rsi), %%rax\n"
        "cmp (%%rdi), %%rax\n"
        "je wait_kbd\n"

        "mov (%%rsi), %%rsi\n"
        "mov %0, %%rsi\n"
        "mov (%%rsi), %%rax\n"
        "mov $0x17, %%rsi\n"
        "mov (%%rsi), %%rbx\n"
        "pop %%rdi\n"
        "pop %%rsi\n"
//        "pop %%rds\n"
        "mov %1, %%ah\n"
        "mov %2, %%al\n"
        "mov %3, %%bl\n"
        :/*list of output parameters*/
        :"m"(adress), "m"(scan), "m"(asci), "m"(status)/*list of input parameters*/
        :"rsi", "rax", "rdi", "rbx", "ah", "al", "bl"/*list of using registers*/
    );

    std::cout << "Adress: " << adress << "; asci: " << asci << "; scan code: " << scan << "; status: " << status << '\n';
  }
  return 0;
}
