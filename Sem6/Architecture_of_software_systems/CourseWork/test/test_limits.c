#include "rtos_api.h"
#include <assert.h>
#include <stdio.h>

DeclareTask(MainTask);

DeclareResource(R0, 0);  DeclareResource(R1, 0);
DeclareResource(R2, 0);  DeclareResource(R3, 0);
DeclareResource(R4, 0);  DeclareResource(R5, 0);
DeclareResource(R6, 0);  DeclareResource(R7, 0);
DeclareResource(R8, 0);  DeclareResource(R9, 0);
DeclareResource(R10, 0); DeclareResource(R11, 0);
DeclareResource(R12, 0); DeclareResource(R13, 0);
DeclareResource(R14, 0); DeclareResource(R15, 0);

DeclareEvent(E0);  DeclareEvent(E1);  DeclareEvent(E2);  DeclareEvent(E3);
DeclareEvent(E4);  DeclareEvent(E5);  DeclareEvent(E6);  DeclareEvent(E7);
DeclareEvent(E8);  DeclareEvent(E9);  DeclareEvent(E10); DeclareEvent(E11);
DeclareEvent(E12); DeclareEvent(E13); DeclareEvent(E14); DeclareEvent(E15);

TASK(MainTask, 1)
{
  int i;
  for (i = 0; i < 16; ++i)
    InitRes(i);
  PIP_GetRes(R0); PIP_ReleaseRes(R0);
  PIP_GetRes(R1); PIP_ReleaseRes(R1);
  PIP_GetRes(R2); PIP_ReleaseRes(R2);
  PIP_GetRes(R3); PIP_ReleaseRes(R3);
  PIP_GetRes(R4); PIP_ReleaseRes(R4);
  PIP_GetRes(R5); PIP_ReleaseRes(R5);
  PIP_GetRes(R6); PIP_ReleaseRes(R6);
  PIP_GetRes(R7); PIP_ReleaseRes(R7);
  PIP_GetRes(R8); PIP_ReleaseRes(R8);
  PIP_GetRes(R9); PIP_ReleaseRes(R9);
  PIP_GetRes(R10); PIP_ReleaseRes(R10);
  PIP_GetRes(R11); PIP_ReleaseRes(R11);
  PIP_GetRes(R12); PIP_ReleaseRes(R12);
  PIP_GetRes(R13); PIP_ReleaseRes(R13);
  PIP_GetRes(R14); PIP_ReleaseRes(R14);
  PIP_GetRes(R15); PIP_ReleaseRes(R15);
  TEventMask mask = E0 | E1 | E2 | E3 | E4 | E5 | E6 | E7 |
                    E8 | E9 | E10 | E11 | E12 | E13 | E14 | E15;
  SetEvent(MainTask, mask);
  printf("Limits test passed\n");
  TerminateTask();
}

int main(void)
{
  StartOS(MainTask);
  return 0;
}
