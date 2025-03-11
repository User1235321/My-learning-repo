#ifndef MYBMPFUNCS_HPP
#define MYBMPFUNCS_HPP

#include <string>
#include <fstream>

void generate16Bit(const std::string & imageName, int width, int height);
void getSizeAndBits(const std::string & imageName, std::ostream & out);
void binarization(const std::string & inImageName, const std::string & outImageName, int binLevel);
void convert16To4(const std::string & inImageName, const std::string & outImageName);

#endif
