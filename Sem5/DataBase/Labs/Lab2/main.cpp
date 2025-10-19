#include <iostream>
#include <cstdlib>

int main()
{
  for (int i = 0; i < 20; ++i)
  {
    std::cout << "INSERT INTO sales VALUES (DEFAULT, " << (std::rand() % 16) << ", " << (std::rand() % 20) << ", '2025-09-" << (i + 10) << "');\n";
  }
  return 0;
}
