#include <iostream>
#include <fstream>
int main()
{
  std::cout << "\033[1;32m";
  std::cout << "Programm start. Press esc to finish";
  std::cout << "\033[0m\n";
  char scan = 0, asci = 0, status = 0;
  int adress = 0;
  std::fstream file("/dev/input/event0");
  while(!file.eof())
  {
    char c = 0;
    file >> c;
    std::cout << unsigned(c) << '\n';
    //std::cout << "Adress: " << adress << "; asci: " << asci << "; scan code: " << scan << "; status: " << status << '\n';
  }
  return 0;
}
