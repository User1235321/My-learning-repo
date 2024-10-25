#include <iostream>

void argumentAdd(long long, long long, long long &)
{
  __asm__
  (
    "leaq 8(%rbp), %rax\n"
    "leaq 16(%rbp), %rbx\n"
    "leaq 24(%rbp), %rsi\n"
    "addq %rbx, %rax\n"
    "movq %rax, (%rsi)\n"
  );
}
void registerAdd()
{
  __asm__
  (
    "addq %rbx, %rax\n"
    "movq %rax, (%rsi)\n"
  );
}
long long gFirst = 12;
long long gSecond = 25;
long long gSum = 0;
void globalAdd()
{
  __asm__
  (
    "movq %0, %%rax\n"
    "movq %1, %%rbx\n"
    "leaq %2, %%rsi\n"
    "addq %%rbx, %%rax\n"
    "movq %%rax, (%%rsi)\n"
    :/*output parameters*/
    :"m"(gFirst), "m"(gSecond), "m"(gSum)
    :"rax", "rbx", "rsi"
  );
}

int main()
{
  long long first = gFirst;
  long long second = gSecond;
  long long sum = first + second;
  std::cout << "\t\t\t" << first << '\t' << second << '\t' << sum << '\n';

  sum = 0;
  argumentAdd(first, second, sum);
  std::cout << "after argumentAdd: \t" << first << '\t' << second << '\t'  << sum << '\n';

  sum = 0;
  __asm__
  (
    "movq %0, %%rax\n"
    "movq %1, %%rbx\n"
    "leaq %2, %%rsi\n"
    :/*output parameters*/
    :"m"(first), "m"(second), "m"(sum)
    :"rax", "rbx", "rsi"
  );
  registerAdd();
  std::cout << "after registerAdd: \t" << first << '\t' << second << '\t'  << sum << '\n';

  globalAdd();
  std::cout << "after globalAdd: \t" << gFirst << '\t' << gSecond << '\t'  << gSum << '\n';

  return 0;
}
