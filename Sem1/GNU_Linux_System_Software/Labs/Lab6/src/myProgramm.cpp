#include <unistd.h>
#include <csignal>
#include <iostream>

void signalHandler( int signum ) {
   std::cout << "Interrupt signal (" << signum << ") received.\n";
   exit(signum);  
}

int main()
{
  long long int PID = fork();
  switch (PID)
  {
    case (-1):
      std::cerr << "Error!";
      return 1;

    case (0):
      //child

      while (true) 
      {
        signal(SIGINT, signalHandler);
        std::cout << "child "; 
        sleep(1000);
        raise(SIGTERM);
      }
      return 0;

    default:
    //parent
    while (true) {
      raise(SIGINT);
      std::cout << "parent";
      sleep(1000);
      signal(SIGTERM, signalHandler);
    }
    return 0;
  }
}
