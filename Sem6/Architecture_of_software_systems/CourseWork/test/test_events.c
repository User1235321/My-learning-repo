#include "rtos_api.h"
#include <assert.h>
#include <stdio.h>

DeclareTask(ConsumerTask);
DeclareTask(ProducerTask);
DeclareEvent(EventA);

volatile int step = 0;

TASK(ProducerTask, 1)
{
  step = 1;
  SetEvent(ConsumerTask, EventA);
  TerminateTask();
}

TASK(ConsumerTask, 2)
{
  ActivateTask(ProducerTask);
  WaitEvent(EventA);
  step = 2;
  printf("Events test passed\n");
  fflush(stdout);
  TerminateTask();
}

int main(void)
{
  StartOS(ConsumerTask);
  return 0;
}
