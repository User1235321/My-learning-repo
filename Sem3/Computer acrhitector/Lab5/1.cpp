#include <iostream>

void argumentAdd(long long & first, long long & second, long long & sum)
{
  sum = 0;
/*  __asm__
  (
    "movq %0, %%rax\n"
    "movq %1, %%rbx\n"
    "movq %2, %%rcx\n"
    "addq %%rax, %%rcx\n"
    "addq %%rbx, %%rcx\n"
    :/output parameters/
    :"m"(first), "m"(second), "m"(sum)
    :"rax", "rbx", "rcx"
  );*/
  __asm__
  (
    "movq 24(%ebp),%rsi\n"
    "movq 16(%ebp),%rax\n"
    "movq 8(%ebp),%rbx\n"
    "addq %rax, %rbx\n"
    "movq %rbx, %rsi\n"
  );
}
void registerAdd()
{

}
long long gFirst = 12;
long long gSecond = 25;
long long gSum = 0;
void globalAdd()
{

}

int main()
{
  long long first = gFirst;
  long long second = gSecond;
  long long sum = first + second;
  std::cout << "\t\t\t" << first << '\t' << second << '\t' << sum << '\n';

  argumentAdd(first, second, sum);
  std::cout << "after argumentAdd: \t" << first << '\t' << second << '\t'  << sum << '\n';

  sum = 0;
  __asm__
  (
    "movq %0, %%rax\n"
    "movq %1, %%rbx\n"
    "movq %2, %%rcx\n"
    :/*output parameters*/
    :"m"(first), "m"(second), "m"(sum)
    :"rax", "rbx", "rcx"
  );
  registerAdd();
  std::cout << "after registerAdd: \t" << first << '\t' << second << '\t'  << sum << '\n';

  globalAdd();
  std::cout << "after globalAdd: \t" << first << '\t' << second << '\t'  << sum << '\n';

  return 0;
}
