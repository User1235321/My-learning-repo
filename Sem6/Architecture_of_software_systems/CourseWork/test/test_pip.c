#include "rtos_api.h"
#include <assert.h>
#include <stdio.h>

DeclareTask(LowTask);
DeclareTask(MediumTask);
DeclareTask(HighTask);
DeclareResource(Res, 0);

volatile int step = 0;

TASK(LowTask, 1)
{
  InitRes(Res);
  PIP_GetRes(Res);
  step = 1;
  ActivateTask(HighTask);
  ActivateTask(MediumTask);
  step = 2;
  PIP_ReleaseRes(Res);
  step = 3;
  TerminateTask();
}

TASK(MediumTask, 2)
{
  step = 4;
  TerminateTask();
}

TASK(HighTask, 3)
{
  step = 5;
  PIP_GetRes(Res);
  step = 6;
  PIP_ReleaseRes(Res);
  step = 7;
  printf("PIP test passed\n");
  fflush(stdout);
  TerminateTask();
}

int main(void)
{
  StartOS(LowTask);
  return 0;
}
