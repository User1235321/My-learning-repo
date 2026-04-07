#include <iostream>
#include <string>
#include <cctype>
#include <cstdlib>

std::string input;
size_t pos;
int token;
int numval;
int depth;

void next()
{
  while (pos < input.size() && std::isspace(input[pos]))
  {
    ++pos;
  }
  if (pos >= input.size())
  {
    token = '$';
    return;
  }
  if (std::isdigit(input[pos]))
  {
    numval = 0;
    while (pos < input.size() && std::isdigit(input[pos]))
    {
      numval = numval * 10 + (input[pos] - '0');
      ++pos;
    }
    token = 'n';
  }
  else
  {
    token = input[pos];
    ++pos;
  }
}

void printIndent()
{
  for (int i = 0; i < depth; ++i)
  {
    std::cout << "  ";
  }
}

int E();
int E_prime(int inh);
int T();
int T_prime(int inh);
int F();

int E()
{
  printIndent();
  std::cout << "E: enter, token=" << char(token) << "\n";
  ++depth;
  int tval = T();
  int res = E_prime(tval);
  --depth;
  printIndent();
  std::cout << "E: return " << res << "\n";
  return res;
}

int E_prime(int inh)
{
  printIndent();
  std::cout << "E': enter, inh=" << inh << ", token=" << char(token) << "\n";
  ++depth;
  int res;
  if (token == '+')
  {
    next();
    int t = T();
    res = E_prime(inh + t);
  }
  else if (token == '-')
  {
    next();
    int t = T();
    res = E_prime(inh - t);
  }
  else
  {
    res = inh;
  }
  --depth;
  printIndent();
  std::cout << "E': return " << res << "\n";
  return res;
}

int T()
{
  printIndent();
  std::cout << "T: enter, token=" << char(token) << "\n";
  ++depth;
  int f = F();
  int res = T_prime(f);
  --depth;
  printIndent();
  std::cout << "T: return " << res << "\n";
  return res;
}

int T_prime(int inh)
{
  printIndent();
  std::cout << "T': enter, inh=" << inh << ", token=" << char(token) << "\n";
  ++depth;
  int res;
  if (token == '*')
  {
    next();
    int f = F();
    res = T_prime(inh * f);
  }
  else if (token == '/')
  {
    next();
    int f = F();
    if (f == 0)
    {
      std::cerr << "Error: division by zero\n";
      std::exit(1);
    }
    res = T_prime(inh / f);
  }
  else
  {
    res = inh;
  }
  --depth;
  printIndent();
  std::cout << "T': return " << res << "\n";
  return res;
}

int F()
{
  printIndent();
  std::cout << "F: enter, token=" << char(token) << "\n";
  ++depth;
  int val;
  if (token == 'n')
  {
    val = numval;
    next();
  }
  else if (token == '(')
  {
    next();
    val = E();
    if (token != ')')
    {
      std::cerr << "Error: expected ')'\n";
      std::exit(1);
    }
    next();
  }
  else if (token == '-')
  {
    next();
    val = -F();
  }
  else
  {
    std::cerr << "Error: unexpected token " << char(token) << "\n";
    std::exit(1);
  }
  --depth;
  printIndent();
  std::cout << "F: return " << val << "\n";
  return val;
}

int main()
{
  std::cout << "Enter expression (end with $): ";
  std::getline(std::cin, input);
  pos = 0;
  next();
  int result = E();
  if (token != '$')
  {
    std::cerr << "Error: unexpected characters after expression\n";
    return 1;
  }
  std::cout << "Result = " << result << "\n";
  return 0;
}
