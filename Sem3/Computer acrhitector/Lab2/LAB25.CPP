#include <dos.h>
#include <bios.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
void main(void)
{
  long long between = 0xB9000000 - 0xB8000000;
  between /= sizeof(int);
  printf("There are %lld ints between 0xB9000 and 0xB8000\n", between);
  FILE * out = fopen("output.txt", "w");
  int * A = (int *)0xB8000000;
  int * B = (int *)0xB9000000;
  fprintf(out, "Array's before assembler replace:\n");
  for (int i = 0; i < 1000; ++i)
  {
    A[i] = rand();
    fprintf(out, "A[%d]: %d; B[%d]: %d\n", i, A[i], i, B[i]);
  }

  asm {
    push es
    push ds
    cld
    lds si, A
    les di, B
    mov cx, 1000
    rep movsw
    pop ds
    pop es
  }
  fprintf(out, "Array's after assembler replace:\n");
  for (i = 0; i < 1000; ++i)
  {
    fprintf(out, "B[%d]: %d\n", 1000, B[i]);
  }
  printf("Programm is finished suceses\n");
  getch();
}


