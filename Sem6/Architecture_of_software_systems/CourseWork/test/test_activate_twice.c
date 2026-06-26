#include "rtos_api.h"
#include <assert.h>
#include <stdio.h>

DeclareTask(TargetTask);
DeclareTask(MainTask);

volatile int count = 0;

TASK(TargetTask, 2)
{
  ++count;
  TerminateTask();
}

TASK(MainTask, 3)
{
  ActivateTask(TargetTask);
  ActivateTask(TargetTask);
  ActivateTask(TargetTask);
  assert(count == 1);
  printf("Activate twice test passed\n");
  TerminateTask();
}

int main(void)
{
  StartOS(MainTask);
  return 0;
}
