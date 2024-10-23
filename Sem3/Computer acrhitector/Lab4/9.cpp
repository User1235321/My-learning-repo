#include <iostream>

void fillArr(int * arr, int size)
{
  for (int i = 0; i < (size * size); ++i)
  {
    arr[i] = i;
  }
}
void printArr(int * arr, int size)
{
  for (int i = 0; i < size; ++i)
  {
    for (int j = 0; j < size; ++j)
    {
      std::cout << "arr[" << i << "][" << j << "] = " << arr[i * size + j] << ' ';
    }
    std::cout << '\n';
  }
}
int main()
{
  int arrSize = 5;
  int arr[arrSize][arrSize];

  fillArr(*arr, arrSize);
  std::cout << "###\nBase array:\n###\n";
  printArr(*arr, arrSize);
  __asm__
  (
    "leaq %0, %%rsi\n"//rsi = arr
    /*"movq %1, %%rbx\n"//rbx = &arrSize
    "movq (%%rbx), %%rax\n"//rax = arrSize
    "movq $0, %%rdx\n"
    "movq $2, %%rcx\n"
    "divq %%rcx\n"// rax = rdx:rax / rcx; rdx = rdx:rax % rcx ###rax = arrSize / 2###
    "movq %%rax, %%rcx\n"//rcx = arrSize / 2*/
    "movq $2, %%rcx\n"//dell

    "mainLoop:\n"
      "push %%rcx\n"
      /*"movq (%%rbx), %%rax\n"
      "movq $0, %%rdx\n"
      "movq $4, %%rcx\n"
      "mulq %%rcx\n"
      "addq %%rax, %%rsi\n"//next array line(*rsi + arrSize * sizeof(int))
      */
      "addq $20, %%rsi\n"//dell
      /*"movq (%%rbx), %%rcx\n"
      "subq $1, %%rcx\n"
      */
      "movq $4, %%rcx\n"
      "localLoop:\n"
        "movq $0, (%%rsi)\n"//*rsi = 0
        "addq $4, %%rsi\n"//*rsi += sizeof(int)
        "loop localLoop\n"
      "addq $4, %%rsi\n"
      "pop %%rcx\n"
      "loop mainLoop\n"
    : /**/
    :"m"(arr), "m"(arrSize)
    :"rsi", "rbx", "rax", "rcx"
  );
  std::cout << "###\nArray after fill1:\n###\n";
  printArr(*arr, arrSize);
  return 0;
}
