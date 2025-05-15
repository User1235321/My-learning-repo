#include <iostream>
#include <cstddef>
#include "string.h"

int main()
{
  rebdev::String str1;
  const char * str2Char = "string2";
  rebdev::String str2(str2Char);
  rebdev::String str3(str2);
  const char * str4Char = "string4";
  rebdev::String str4(str4Char);

  std::cout << "compare 2 and 3: " << str2.compare(str3) << '\n';
  std::cout << "compare 2 and 4: " << str2.compare(str4) << '\n';

  rebdev::String str7(str2.insert(2, "ok"));
  rebdev::String str8(str7 + str3);
  size_t i = 0;

  std::cout << "insert ok in 2:";
  while (str7[i] != '\0')
  {
    std::cout << str7[i];
    i += 1;
  }
  i = 0;

  std::cout << "7 + 3:";
  while (str8[i] != '\0')
  {
    std::cout << str8[i];
    i += 1;
  }
  i = 0;

  return 0;
}
