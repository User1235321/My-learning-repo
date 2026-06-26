#include "rtos_api.h"
#include <stdio.h>

DeclareTask(TestTask);
DeclareResource(Res, 0);

TASK(TestTask, 1)
{
  InitRes(Res);
  PIP_GetRes(Res);
  printf("Terminate in critical section test passed\n");
  TerminateTask();
}

int main(void)
{
  StartOS(TestTask);
  return 0;
}
