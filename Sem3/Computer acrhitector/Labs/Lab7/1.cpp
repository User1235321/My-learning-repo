#include<iostream>

int main()
{
  float a = 1.98, b = 2.35, c = 0;
  __asm__
  (
    "finit\n"//Init FPU
    "flds %0\n"//a to FPU stack
    "flds %1\n"//b to FPU stack
    "faddp\n"//add two numbers from stack and put result to stack
    "fstps %2\n"//add highter stack element to c
    :/*list of output parameters*/
    :"m"(a), "m"(b), "m"(c)/*list of input parameters*/
    :/*list of using registers*/
  );
  std::cout << a << " + " << b << " = " << c << '\n';

  __asm__
  (
    "fstps %0\n"
    :/*list of output parameters*/
    :"m"(c)/*list of input parameters*/
    :/*list of using registers*/
  );
  std::cout << "Get nan: " << c << '\n';

  int ctrl = 0x027A;
	a = 128.32;
	b = 0;
	__asm__
	(
		"finit\n"
		"fldcw %0\n"
		"fld %2\n"
		"fld %1\n"
		"fdiv\n"
		"fstps %3\n"
    :/*list of output parameters*/
    :"m" (ctrl), "m"(a), "m"(b), "m"(c)/*list of input parameters*/
    :/*list of using registers*/
  );
	std::cout << "Result of division on zero: " << c << "\n";

  return 0;
}
