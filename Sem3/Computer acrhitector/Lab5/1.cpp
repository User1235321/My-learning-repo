#include <iostream>
#include <bitset>
#include <cstdlib>
#include <cmath>

void printOut(unsigned long long f, int s, unsigned long long r)
{
  std::cout << "***\nTen\n***\n" << "first: \t\t" << f << "\nsecond: \t" << s << "\nresult: \t" << r << '\n';
  std::bitset< sizeof(unsigned long long) * 8 > fb(f);
  std::bitset< sizeof(unsigned long long) * 8 > rb(r);
  std::cout << "***\nBin\n***\n" << "first: \t\t" << fb << "\nresult: \t" << rb << '\n';
}
void shlWithParam(unsigned long long, int, unsigned long long &)
{
  //Parametrs of function in Linux saves in rdi, rsi, rdx, rcx
  __asm__
  (
    "movl %edi, %ebx\n"//Put right part of first param in ebx
    "shr $32, %rdi\n"//Put left part of rdi to edi
    "movl %edi, %eax\n"//Put left part of first param in eax
    "movl %esi, %ecx\n"//Put second param in ecx
    "movl %ebx, %esi\n"//Put left part of first param in esi
    //eax = first[0-4]; ebx = first[5-8]; ecx = second; esi = ebx = first[5-8];

    "shl %ecx, %eax\n"//eax = eax << ecx
    "shr %ecx, %esi\n"//esi = esi >> ecx
    "andl %esi, %eax\n"//eax = eax || esi
    "shl %ecx, %ebx\n"//ebx = ebx << ecx

    "movl %eax, %ecx\n"//Put eax in right part of rcx
    "shl $32, %rcx\n"//Right part of rcx now left
    "movl %ebx, %ecx\n"// Put ebx in right part of rcx
    "movq %rcx, (%rdx)\n"//Put rcx in rdx
  );
}

int main()
{
  srand(time(0));
  unsigned long long first = rand();
  for (size_t i = 0; i < (rand() % 32); ++i) first *= 10;
  first += rand();
  int second = (rand () % 8);
  unsigned long long result = 0;
  std::cout << "Start data:\n";
  printOut(first, second, result);

  shlWithParam(first, second, result);
  std::cout << "\n\n\nAfter shl with parameters:\n";
  printOut(first, second, result);
  return 0;
}
