#include <dos.h>
#include <bios.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
void main(void)
{
  const int arraySize = 10;
  int A[arraySize], B[arraySize];
  printf("Array's before assembler replace:\n");
  for (int i = 0; i < arraySize; ++i)
  {
    printf("A[%d]: %d; B[%d]: %d\n", i, A[i], i, B[i]);
  }
  asm {
    lea si, A
    lea bx, B
    mov cx, arraySize
  };
  loop_start:
  asm {
      mov ax, ss:[si]
      mov ss:[bx], ax
      add si, 2
      add bx, 2
      loop loop_start
  };
  printf("Array's after assembler replace:\n");
  for (i = 0; i < arraySize; ++i)
  {
    printf("B[%d]: %d\n", i, B[i]);
  }
  getch();
}

