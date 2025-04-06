#include "third.hpp"

#include <cmath>

#include "vcl/vectorclass.h"
#include "myFunc.hpp"

void vec16c(std::ostream & out)
{
  unsigned char orig[2][16];
  for (unsigned char i = 0; i < 16; ++i)
  {
    orig[0][i] = i;
    orig[1][i] = i * 2;
  }
  out << "\033[1;34mVec16c sub:\033[0m\nBegin value:\n";
  print< unsigned char >(out, 16, 2, &orig[0][0]);

  Vec16c firstVec, secondVec, resVec;
  firstVec.load(orig[0]);
  secondVec.load(orig[1]);
  resVec = secondVec - firstVec;
  unsigned char res[1][16];
  resVec.store(res);

  out << "Result:\n";
  print< unsigned char >(out, 16, 1, &res[0][0]);
}
void vec4f(std::ostream & out)
{
  float orig[2][4];
  for (int i = 0; i < 4; ++i)
  {
    orig[0][i] = (float)i + ((float)i / 10.0);
    orig[1][i] = (float)i * 4.5;
  }
  out << "\033[1;34mVec4f mul:\033[0m\nBegin value:\n";
  print< float >(out, 4, 2, &orig[0][0]);

  Vec4f firstVec, secondVec, resVec;
  firstVec.load(orig[0]);
  secondVec.load(orig[1]);
  resVec = secondVec * firstVec;
  float res[1][4];
  resVec.store(res[0]);

  out << "Result:\n";
  print< float >(out, 4, 1, &res[0][0]);
}
void vec8f(std::ostream & out)
{
  float orig[2][8];
  for (int i = 0; i < 8; ++i)
  {
    orig[0][i] = (float)i * 12.0 - 4 ;
    orig[1][i] = (float)i + std::sin(i * i);
  }
  out << "\033[1;34mVec8f div:\033[0m\nBegin value:\n";
  print< float >(out, 8, 2, &orig[0][0]);

  Vec8f firstVec, secondVec, resVec;
  firstVec.load(orig[0]);
  secondVec.load(orig[1]);
  resVec = firstVec / secondVec;
  float res[1][8];
  resVec.store(res[0]);

  out << "Result:\n";
  print< float >(out, 8, 1, &res[0][0]);
}
