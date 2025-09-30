#include <unistd.h>
#include <iostream>
#include <sys/wait.h>

int main()
{
  int status;
	if (fork() == 0)
  {
    std::cout << "Son process. PID: " << getpid() << "; PPID: " << getppid() << '\n';
    std::cout << "Son die\n";
  }
  else
  {
    std::cout << "Father process. PID: " << getpid() << "; PPID: " << getppid() << '\n';
    sleep(5);
    std::cout << "Father die\n";
  }
  return 0;
}
