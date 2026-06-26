#include "rtos_api.h"
#include <assert.h>
#include <stdio.h>

DeclareTask(LowTask);
DeclareTask(HighTask);

volatile int trace[4];
volatile int idx = 0;

TASK(LowTask, 1)
{
  trace[idx++] = 1;
  ActivateTask(HighTask);
  trace[idx++] = 2;
  assert(idx == 3);
  assert(trace[0] == 1);
  assert(trace[1] == 3);
  assert(trace[2] == 2);
  printf("RMA test passed\n");
  TerminateTask();
}

TASK(HighTask, 3)
{
  trace[idx++] = 3;
  TerminateTask();
}

int main(void)
{
  StartOS(LowTask);
  return 0;
}
