#include <iostream>
#include <stdexcept>

#include "myBMPFuncs.hpp"

int main()
{
  try
  {
    generate16Bit("baseImage.bmp", 513, 513);
    std::cout << "\033[1;32mbaseImage.bmp successfully created\033[0m\n";

    std::cout << "\033[1;32m" << "1. baseImage.bmp characteristics:\033[0m\n";
    getSizeAndBits("baseImage.bmp", std::cout);

    std::cout << "\033[1;32m2. enter binarization file name:\033[0m\n";
    std::string imageName;
    std::cin >> imageName;
    std::cout << "\033[1;32mEnter binarization level:\033[0m\n";
    int binLevel = 0;
    std::cin >> binLevel;
    std::string binImageName = "bin" + imageName;
    binarization(imageName, binImageName, binLevel);
    std::cout << "\033[1;32m" << imageName << " successfully binarizated\033[0m\n";

    std::cout << "\033[1;32m" << "3. 16->4:\033[0m\n";
    convert16To4("baseImage.bmp", "4bitBaseImage.bmp");
  }
  catch (const std::logic_error & e)
  {
    std::cerr << "\033[1;31m" << e.what() << "\033[0m\n";
  }
  return 0;
}
