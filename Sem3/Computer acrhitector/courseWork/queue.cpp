#include <iostream>
#include <string>

int main()
{
  std::cout << "\033[1;33mQueue programm had started!\nYou have next options: PUSH NUM / POP NUM\nTo exit print \"exit\"\033[0m\n";
  std::string input;
  std::getline(std::cin, input);
  while (input.find("exit") == std::string::npos)
  {
    if (input.find("pop") != std::string::npos)
    {
      __asm__
      (
        "xor %eax, %eax\n"
      );
    }
    else if(input.find("push") != std::string::npos)
    {
      __asm__
      (
        "xor %eax, %eax\n"
      );
    }
    else
    {
      std::cout << "\033[1;31mBad input!\033[0m\n";
    }
    std::getline(std::cin, input);
  }
  std::cout << "\033[1;32mProgramm is ended\033[0m\n";
  return 0;
}
