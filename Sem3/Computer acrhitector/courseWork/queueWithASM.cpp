#include <iostream>
#include <string>

int main()
{
  std::cout << "\033[1;33mQueue programm had started!\nYou have next options: PUSH NUM / POP\nTo exit print \"exit\"\033[0m\n";
  std::string input;
  std::getline(std::cin, input);
  size_t size = 100;
  long long * queue = new long long[size];
  long long head = 0;

  while (input.find("exit") == std::string::npos)
  {
    if (input.find("pop") != std::string::npos)
    {
      if (head == 0)
      {
        std::cout << "\033[1;31mQueue is empty!\033[0m\n";
      }
      else
      {
        std::cout << *queue << '\n';
        __asm__
        (
          "leaq %0, %%rsi\n"
          "subq $1, (%%rsi)\n"
          "leaq %1, %%rsi\n"
          "movq %0, %%rcx\n"

          "LoopMain:\n"
            "jrcxz exit\n"
            "movq 8(%%rsi), %%rax\n"
            "movq %%rax, (%%rsi)\n"
            "addq $8, %%rsi\n"
          "loop LoopMain\n"
          "exit:\n"
          ://list of output parameters
          :"m" (head), "m"(*queue)//list of input parameters
          :"rsi", "rcx", "rax"//list of using registers
        );
      }
    }
    else if(input.find("push") != std::string::npos)
    {
      try
      {
        long long in = std::stoll(input.substr(input.find("push") + 5));
        __asm__
        (
          "leaq %0, %%rsi\n"
          "movq %1, %%rax\n"
          "movq $8, %%rdx\n"
          "mulq %%rdx\n"
          "addq %%rax, %%rsi\n"
          "movq %2, %%rax\n"
          "movq %%rax, (%%rsi)\n"
          "leaq %1, %%rsi\n"
          "addq $1, (%%rsi)\n"
          ://list of output parameters
          :"m" (*queue), "m"(head), "m"(in)//list of input parameters
          :"rsi", "rax", "rdx"//list of using registers
        );
        if (head == size)
        {
          long long * q = new long long[size * 2];
          for (size_t i = 0; i < size; ++i)
          {
            q[i] = queue[i];
          }
          delete[] queue;
          queue = q;
          q = nullptr;
          size *= 2;
        }
        std::cout << "\033[1;32m" << in << " successfull added to queue\033[0m\n";
      }
      catch(...)
      {
        std::cout << "\033[1;31mBad push input!\033[0m\n";
      }
    }
    else if(input.find("show") != std::string::npos)
    {
      for(size_t i = 0; i < head; ++i)
      {
        std::cout << *(queue + i) << ' ';
      }
      std::cout << '\n';
    }
    else
    {
      std::cout << "\033[1;31mBad input!\033[0m\n";
    }
    std::getline(std::cin, input);
  }

  std::cout << "\033[1;32mProgramm is ended\033[0m\n";
  delete[] queue;
  return 0;
}
