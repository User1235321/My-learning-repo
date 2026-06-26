#include "rtos_api.h"
#include <assert.h>
#include <stdio.h>

DeclareTask(MainTask);
DeclareResource(R0, 0); DeclareResource(R1, 0); DeclareResource(R2, 0); DeclareResource(R3, 0);
DeclareResource(R4, 0); DeclareResource(R5, 0); DeclareResource(R6, 0); DeclareResource(R7, 0);
DeclareResource(R8, 0);

volatile int step = 0;

TASK(MainTask, 5)
{
  InitRes(R0); InitRes(R1); InitRes(R2); InitRes(R3);
  InitRes(R4); InitRes(R5); InitRes(R6); InitRes(R7);
  InitRes(R8);

  PIP_GetRes(R0); step = 1;
  PIP_GetRes(R1); step = 2;
  PIP_GetRes(R2); step = 3;
  PIP_GetRes(R3); step = 4;
  PIP_GetRes(R4); step = 5;
  PIP_GetRes(R5); step = 6;
  PIP_GetRes(R6); step = 7;
  PIP_GetRes(R7); step = 8;

  PIP_ReleaseRes(R7); step = 9;
  PIP_ReleaseRes(R6); step = 10;
  PIP_ReleaseRes(R5); step = 11;
  PIP_ReleaseRes(R4); step = 12;
  PIP_ReleaseRes(R3); step = 13;
  PIP_ReleaseRes(R2); step = 14;
  PIP_ReleaseRes(R1); step = 15;
  PIP_ReleaseRes(R0); step = 16;
  assert(step == 16);
  printf("Nested resources test passed\n");
  TerminateTask();
}

int main(void)
{
  StartOS(MainTask);
  return 0;
}
