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
        head -= 1;
        for (size_t i = 0; i < head; ++i)
        {
          *(queue + i) = *(queue + i + 1);
        }
      }
    }
    else if(input.find("push") != std::string::npos)
    {
      try
      {
        long long in = std::stoll(input.substr(input.find("push") + 5));
        *(queue + head) = in;
        head += 1;
        std::cout << "\033[1;32m" << in << " successfull added to queue\033[0m\n";
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
      }
      catch(...)
      {
        std::cout << "\033[1;31mBad push input!\033[0m\n";
      }
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
