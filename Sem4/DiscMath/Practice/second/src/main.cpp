#include <iostream>
#include <string>
#include <list>
#include <utility>

int main()
{
  std::list< std::pair<std::string, std::string> > rules
  {
    {"9", "8|"},
    {"8", "7|"},
    {"7", "6|"},
    {"6", "5|"},
    {"5", "4|"},
    {"4", "3|"},
    {"3", "2|"},
    {"2", "1|"},
    {"1", "0|"},
    {"|9", "9||||||||||"},
    {"|8", "8||||||||||"},
    {"|7", "7||||||||||"},
    {"|6", "6||||||||||"},
    {"|5", "5||||||||||"},
    {"|4", "4||||||||||"},
    {"|3", "3||||||||||"},
    {"|2", "2||||||||||"},
    {"|1", "1||||||||||"},
    {"|0", "0||||||||||"},
    {"0", ""}
  };
  std::string input;
  std::cout << "Write decimal number:\n\n";
  std::cin >> input;
  size_t steps = 1;
  for (auto iter : rules)
  {
    while (input.find(iter.first) != std::string::npos)
    {
      std::cout << steps++ << ". " << input << '\n';
      input.replace(input.find(iter.first), iter.first.size(), iter.second);
    }
  }
  std::cout << "\nResult: " << input << '\n';
  std::cout << "Size: " << input.size() << '\n';
  return 0;
}
