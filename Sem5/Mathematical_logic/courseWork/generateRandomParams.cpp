#include <iostream>
#include <fstream>
#include <ctime>

int main()
{
  std::srand(time(0));
  std::ofstream out("params.txt");
  for (size_t i = 0; i < 7; ++i)
  {
    out << std::rand() % 9 << ' ' << std::rand() % 4 << ' ' << std::rand() % 9 << '\n';
  }
  for (size_t i = 0; i < 4; ++i)
  {
    out << std::rand() % 4 << ' ' << std::rand() % 9 << ' ' << std::rand() % 4 << ' ' << std::rand() % 4 << '\n';
  }
  for (size_t i = 0; i < 2; ++i)
  {
    out << std::rand() % 4 << ' ' << std::rand() % 9 << ' ' << std::rand() % 4 << ' ' << std::rand() % 4 << '\n';
  }
  for (size_t i = 0; i < 6; ++i)
  {
    out << std::rand() % 4 << ' ' << std::rand() % 9 << ' ' << std::rand() % 4 << ' ' << std::rand() % 4 << '\n';
  }
}
