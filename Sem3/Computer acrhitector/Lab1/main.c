#include <dos.h>
#include <bios.h>
#include <stdio.h>
#include <conio.h>
#define PortCan0 0x40
void beep(unsigned iTone,unsigned iDlit);
void main(void)
{
  long int lCnt=0;
  int iA=0x1234;
  char far * pT=(char *)0x46C;
  printf ("\n Печатаем 10 раз значение байта с известным адресом \n");
  for (int i=0; i<10; i++)
    printf (" \n %d ", * pT);
  printf ("\n Для продолжения нажмите любую клавишу \n");
  getch();
  printf ("\n Читаем содержимое порта с адресом 40 с помощью функции Си \n" );
  printf ("\n Для выхода из цикла - нажмите любую клавишу \n");
  while (bioskey(1)==0)
  {
    printf (" \n Порт40 = %d ", inp(PortCan0));
    delay(500);
  }
  getch();
  printf ("\n Читаем содержимое порта с адресом 40 ассемблером \n" );
  while (bioskey(1) == 0 )
  {
    asm { push ax
      in al,0x40
    }
    unsigned char Tmm = _AL;
    mov Tmm,al
    asm
      pop ax
    delay (500);
    printf (" \n Порт40 = %d ", Tmm );
  }
  getch();
  printf ("\n Для продолжения - нажмите любую клавишу \n");
  getch();
  long far * pTime=(long *)0x46C;
  while (bioskey(1) == 0)
  {
    printf ("\n %ld",*pTime);
    delay(1000);
  }
  getch();
  int Time;
  while (bioskey(1) == 0)
  {
    asm push ds
    asm push si
    asm mov ax,40h
    asm mov ds,ax
    asm mov si,0x6C
    asm mov ax,[ds:si]
    asm mov Time,ax
    asm pop si
    asm pop ds
    printf ("\n %d",Time);
    delay(300);
  }
  beep(400,200);
  for ( lCnt=0; lCnt<1000000; lCnt++)
  {
  a1:
    asm { mov ax,iA
          mov ax,iA
          mov ax,iA
          mov ax,iA
          mov ax,iA
          mov ax,iA
          mov ax,iA
          mov ax,iA
          mov ax,iA
          a2:
            mov ax,iA }
  }
  beep(400,200);
}
void beep(unsigned iTone,unsigned iDlit)
{
  sound(iTone);
  delay(iDlit);
  nosound();
}
