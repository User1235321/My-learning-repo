#include <iostream>
#include <string>

std::string input;
size_t pos;

void next()
{
  ++pos;
}

char current()
{
  if (pos < input.size()) return input[pos];
  return 0;
}

bool match(char expected)
{
  if (current() == expected)
  {
    next();
    return true;
  }
  return false;
}

void printIndent(int depth)
{
  for (int i = 0; i < depth; ++i) std::cout << "  ";
}

bool parseS(int depth);
bool parseE(int depth);
bool parseA(int depth);
bool parseT(int depth);
bool parseB(int depth);
bool parseF(int depth);

bool parseS(int depth)
{
  printIndent(depth); std::cout << "S: enter, current = " << current() << "\n";
  if (!parseE(depth + 1)) { printIndent(depth); std::cout << "S: fail\n"; return false; }
  if (!match('$')) { printIndent(depth); std::cout << "S: fail (expected $)\n"; return false; }
  printIndent(depth); std::cout << "S: success\n";
  return true;
}

bool parseE(int depth)
{
  printIndent(depth); std::cout << "E: enter, current = " << current() << "\n";
  if (!parseT(depth + 1)) { printIndent(depth); std::cout << "E: fail\n"; return false; }
  if (!parseA(depth + 1)) { printIndent(depth); std::cout << "E: fail\n"; return false; }
  printIndent(depth); std::cout << "E: success\n";
  return true;
}

bool parseA(int depth)
{
  printIndent(depth); std::cout << "A: enter, current = " << current() << "\n";
  if (current() == '+')
  {
    match('+');
    if (!parseT(depth + 1)) { printIndent(depth); std::cout << "A: fail\n"; return false; }
    if (!parseA(depth + 1)) { printIndent(depth); std::cout << "A: fail\n"; return false; }
    printIndent(depth); std::cout << "A: success (+\n";
    return true;
  }
  else if (current() == ')' || current() == '$')
  {
    printIndent(depth); std::cout << "A: success (epsilon)\n";
    return true;
  }
  printIndent(depth); std::cout << "A: fail\n";
  return false;
}

bool parseT(int depth)
{
  printIndent(depth); std::cout << "T: enter, current = " << current() << "\n";
  if (!parseF(depth + 1)) { printIndent(depth); std::cout << "T: fail\n"; return false; }
  if (!parseB(depth + 1)) { printIndent(depth); std::cout << "T: fail\n"; return false; }
  printIndent(depth); std::cout << "T: success\n";
  return true;
}

bool parseB(int depth)
{
  printIndent(depth); std::cout << "B: enter, current = " << current() << "\n";
  if (current() == '*')
  {
    match('*');
    if (!parseF(depth + 1)) { printIndent(depth); std::cout << "B: fail\n"; return false; }
    if (!parseB(depth + 1)) { printIndent(depth); std::cout << "B: fail\n"; return false; }
    printIndent(depth); std::cout << "B: success (*)\n";
    return true;
  }
  else if (current() == '+' || current() == ')' || current() == '$')
  {
    printIndent(depth); std::cout << "B: success (epsilon)\n";
    return true;
  }
  printIndent(depth); std::cout << "B: fail\n";
  return false;
}

bool parseF(int depth)
{
  printIndent(depth); std::cout << "F: enter, current = " << current() << "\n";
  if (current() == 'i')
  {
    match('i');
    printIndent(depth); std::cout << "F: success (i)\n";
    return true;
  }
  else if (current() == '(')
  {
    match('(');
    if (!parseE(depth + 1)) { printIndent(depth); std::cout << "F: fail\n"; return false; }
    if (!match(')')) { printIndent(depth); std::cout << "F: fail (expected ))\n"; return false; }
    printIndent(depth); std::cout << "F: success ((E))\n";
    return true;
  }
  printIndent(depth); std::cout << "F: fail\n";
  return false;
}

int main()
{
  std::cout << "Enter expression (end with $): ";
  std::getline(std::cin, input);
  pos = 0;
  if (parseS(0) && pos == input.size())
    std::cout << "ACCEPT\n";
  else
    std::cout << "REJECT\n";
  return 0;
}
