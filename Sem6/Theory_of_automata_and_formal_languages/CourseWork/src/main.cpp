#include "parser.h"
#include <iostream>
#include <cstdlib>

void printHelp()
{
  std::cout << "Usage: cmilan input_file\n";
}

int main(int argc, char** argv)
{
  if (argc < 2)
  {
    printHelp();
    return EXIT_FAILURE;
  }

  std::ifstream input;
  input.open(argv[1]);

  if (input)
  {
    Parser p(argv[1], input);
    p.parse();
    return EXIT_SUCCESS;
  }
  else
  {
    std::cerr << "File '" << argv[1] << "' not found\n";
    return EXIT_FAILURE;
  }
}
